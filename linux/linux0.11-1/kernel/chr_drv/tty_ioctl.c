/* passed
* linux/kernel/chr_drv/tty_ioctl.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <termios.h>		// �ն������������ͷ�ļ�����Ҫ��������첽ͨ�ſڵ��ն˽ӿڡ�

#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/tty.h>		// tty ͷ�ļ����������й�tty_io������ͨ�ŷ���Ĳ�����������

#include <asm/io.h>		// io ͷ�ļ�������Ӳ���˿�����/���������䡣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <asm/system.h>		// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ

// ���ǲ������������飨���Ϊ�������飩���������벨�������ӵĶ�Ӧ��ϵ�μ��б���˵����
static unsigned short quotient[] = {
	0, 2304, 1536, 1047, 857,
	768, 576, 384, 192, 96,
	64, 48, 24, 12, 6, 3
};

//// �޸Ĵ������ʡ�
// ������tty - �ն˶�Ӧ��tty ���ݽṹ��
// �ڳ��������־DLAB(��·���ƼĴ���λ7)��λ����£�ͨ���˿�0x3f8 ��0x3f9 ��UART �ֱ�д��
// ���������ӵ��ֽں͸��ֽڡ�
static void
change_speed (struct tty_struct *tty)
{
	unsigned short port, quot;

// ���ڴ����նˣ���tty �ṹ�Ķ��������data �ֶδ�ŵ��Ǵ��ж˿ں�(0x3f8 ��0x2f8)��
	if (!(port = tty->read_q.data))
		return;
// ��tty ��termios �ṹ����ģʽ��־����ȡ�����õĲ����������ţ��ݴ˴Ӳ���������������ȡ��
// ��Ӧ�Ĳ���������ֵ��CBAUD �ǿ���ģʽ��־���в�����λ�����롣
	quot = quotient[tty->termios.c_cflag & CBAUD];
	cli ();			// ���жϡ�
	outb_p (0x80, port + 3);	/* set DLAB */// �������ó���������־DLAB��
	outb_p (quot & 0xff, port);	/* LS of divisor */// ������ӵ��ֽڡ�
	outb_p (quot >> 8, port + 1);	/* MS of divisor */// ������Ӹ��ֽڡ�
	outb (0x03, port + 3);	/* reset DLAB */// ��λDLAB��
	sti ();			// ���жϡ�
}

//// ˢ��tty ������С�
// ������gueue - ָ���Ļ������ָ�롣
// �����е�ͷָ�����βָ�룬�Ӷ��ﵽ��ջ�����(���ַ�)��Ŀ�ġ�
static void
flush (struct tty_queue *queue)
{
	cli ();
	queue->head = queue->tail;
	sti ();
}

//// �ȴ��ַ����ͳ�ȥ��
static void
wait_until_sent (struct tty_struct *tty)
{
/* do nothing - not implemented *//* ʲô��û�� - ��δʵ�� */
}

//// ����BREAK ���Ʒ���
static void
send_break (struct tty_struct *tty)
{
/* do nothing - not implemented *//* ʲô��û�� - ��δʵ�� */
}

//// ȡ�ն�termios �ṹ��Ϣ��
// ������tty - ָ���ն˵�tty �ṹָ�룻termios - �û�������termios �ṹ������ָ�롣
// ����0 ��
static int
get_termios (struct tty_struct *tty, struct termios *termios)
{
	int i;

// ������֤һ���û��Ļ�����ָ����ָ�ڴ����Ƿ��㹻���粻��������ڴ档
	verify_area (termios, sizeof (*termios));
// ����ָ��tty �ṹ�е�termios �ṹ��Ϣ���û� termios �ṹ��������
	for (i = 0; i < (sizeof (*termios)); i++)
		put_fs_byte (((char *) &tty->termios)[i], i + (char *) termios);
	return 0;
}

//// �����ն�termios �ṹ��Ϣ��
// ������tty - ָ���ն˵�tty �ṹָ�룻termios - �û�������termios �ṹָ�롣
// ����0 ��
static int
set_termios (struct tty_struct *tty, struct termios *termios)
{
	int i;

// ���ȸ����û���������termios �ṹ��Ϣ��ָ��tty �ṹ�С�
	for (i = 0; i < (sizeof (*termios)); i++)
		((char *) &tty->termios)[i] = get_fs_byte (i + (char *) termios);
// �û��п������޸���tty �Ĵ��пڴ��䲨���ʣ����Ը���termios �ṹ�еĿ���ģʽ��־c_cflag
// �޸Ĵ���оƬUART �Ĵ��䲨���ʡ�
	change_speed (tty);
	return 0;
}

//// ��ȡtermio �ṹ�е���Ϣ��
// ������tty - ָ���ն˵�tty �ṹָ�룻termio - �û�������termio �ṹ������ָ�롣
// ����0��
static int
get_termio (struct tty_struct *tty, struct termio *termio)
{
	int i;
	struct termio tmp_termio;

// ������֤һ���û��Ļ�����ָ����ָ�ڴ����Ƿ��㹻���粻��������ڴ档
	verify_area (termio, sizeof (*termio));
// ��termios �ṹ����Ϣ���Ƶ�termio �ṹ�С�Ŀ����Ϊ������ģʽ��־�������ͽ���ת����Ҳ��
// ��termios �ĳ���������ת��Ϊtermio �Ķ��������͡�
	tmp_termio.c_iflag = tty->termios.c_iflag;
	tmp_termio.c_oflag = tty->termios.c_oflag;
	tmp_termio.c_cflag = tty->termios.c_cflag;
	tmp_termio.c_lflag = tty->termios.c_lflag;
// ���ֽṹ��c_line ��c_cc[]�ֶ�����ȫ��ͬ�ġ�
	tmp_termio.c_line = tty->termios.c_line;
	for (i = 0; i < NCC; i++)
		tmp_termio.c_cc[i] = tty->termios.c_cc[i];
// �����ָ��tty �ṹ�е�termio �ṹ��Ϣ���û� termio �ṹ��������
	for (i = 0; i < (sizeof (*termio)); i++)
		put_fs_byte (((char *) &tmp_termio)[i], i + (char *) termio);
	return 0;
}

/*
* This only works as the 386 is low-byt-first
*/
/*
* �����termio ���ú�������386 ���ֽ���ǰ�ķ�ʽ�¿��á�
*/
//// �����ն�termio �ṹ��Ϣ��
// ������tty - ָ���ն˵�tty �ṹָ�룻termio - �û�������termio �ṹָ�롣
// ���û�������termio ����Ϣ���Ƶ��ն˵�termios �ṹ�С�����0 ��
static int
set_termio (struct tty_struct *tty, struct termio *termio)
{
	int i;
	struct termio tmp_termio;

// ���ȸ����û���������termio �ṹ��Ϣ����ʱtermio �ṹ�С�
	for (i = 0; i < (sizeof (*termio)); i++)
		((char *) &tmp_termio)[i] = get_fs_byte (i + (char *) termio);
// �ٽ�termio �ṹ����Ϣ���Ƶ�tty ��termios �ṹ�С�Ŀ����Ϊ������ģʽ��־�������ͽ���ת����
// Ҳ����termio �Ķ���������ת����termios �ĳ��������͡�
	*(unsigned short *) &tty->termios.c_iflag = tmp_termio.c_iflag;
	*(unsigned short *) &tty->termios.c_oflag = tmp_termio.c_oflag;
	*(unsigned short *) &tty->termios.c_cflag = tmp_termio.c_cflag;
	*(unsigned short *) &tty->termios.c_lflag = tmp_termio.c_lflag;
// ���ֽṹ��c_line ��c_cc[]�ֶ�����ȫ��ͬ�ġ�
	tty->termios.c_line = tmp_termio.c_line;
	for (i = 0; i < NCC; i++)
		tty->termios.c_cc[i] = tmp_termio.c_cc[i];
// �û��������޸���tty �Ĵ��пڴ��䲨���ʣ����Ը���termios �ṹ�еĿ���ģʽ��־��c_cflag
// �޸Ĵ���оƬUART �Ĵ��䲨���ʡ�
	change_speed (tty);
	return 0;
}

//// tty �ն��豸��ioctl ������
// ������dev - �豸�ţ�cmd - ioctl ���arg - ��������ָ�롣
int
tty_ioctl (int dev, int cmd, int arg)
{
	struct tty_struct *tty;
// ����ȡtty �����豸�š�������豸����5(tty �ն�)������̵�tty �ֶμ������豸�ţ��������
// ��tty ���豸���Ǹ����������ý���û�п����նˣ�Ҳ�����ܷ�����ioctl ���ã�����������
	if (MAJOR (dev) == 5)
	{
		dev = current->tty;
		if (dev < 0)
			panic ("tty_ioctl: dev<0");
// ����ֱ�Ӵ��豸����ȡ�����豸�š�
	}
	else
		dev = MINOR (dev);
// ���豸�ſ�����0(����̨�ն�)��1(����1 �ն�)��2(����2 �ն�)��
// ��tty ָ���Ӧ���豸�ŵ�tty �ṹ��
	tty = dev + tty_table;
// ����tty ��ioctl ������зֱ���
	switch (cmd)
	{
	case TCGETS:
//ȡ��Ӧ�ն�termios �ṹ�е���Ϣ��
		return get_termios (tty, (struct termios *) arg);
	case TCSETSF:
// ������termios ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ����꣬����ˢ��(���)������С�
// �����á�
		flush (&tty->read_q);	/* fallthrough */
	case TCSETSW:
// �������ն�termios ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ�����(�ľ�)�������޸Ĳ���
// ��Ӱ����������������Ҫʹ��������ʽ��
		wait_until_sent (tty);	/* fallthrough */
	case TCSETS:
// ������Ӧ�ն�termios �ṹ�е���Ϣ��
		return set_termios (tty, (struct termios *) arg);
	case TCGETA:
// ȡ��Ӧ�ն�termio �ṹ�е���Ϣ��
		return get_termio (tty, (struct termio *) arg);
	case TCSETAF:
// ������termio ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ����꣬����ˢ��(���)������С�
// �����á�
		flush (&tty->read_q);	/* fallthrough */
	case TCSETAW:
// �������ն�termio ����Ϣ֮ǰ����Ҫ�ȵȴ�����������������ݴ�����(�ľ�)�������޸Ĳ���
// ��Ӱ����������������Ҫʹ��������ʽ��
		wait_until_sent (tty);	/* fallthrough *//* ����ִ�� */
	case TCSETA:
// ������Ӧ�ն�termio �ṹ�е���Ϣ��
		return set_termio (tty, (struct termio *) arg);
	case TCSBRK:
// �ȴ�������д������(��)���������ֵ��0������һ��break��
		if (!arg)
		{
			wait_until_sent (tty);
			send_break (tty);
		}
		return 0;
	case TCXONC:
// ��ʼ/ֹͣ���ơ��������ֵ��0�����������������1�������¿������������������2�������
// ���룻�����3�������¿�����������롣
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TCFLSH:
//ˢ����д�������û���ͻ����յ���û�ж����ݡ����������0����ˢ��(���)������У������1��
// ��ˢ��������У������2����ˢ�������������С�
		if (arg == 0)
			flush (&tty->read_q);
		else if (arg == 1)
			flush (&tty->write_q);
		else if (arg == 2)
		{
			flush (&tty->read_q);
			flush (&tty->write_q);
		}
		else
			return -EINVAL;
		return 0;
	case TIOCEXCL:
// �����ն˴�����·ר��ģʽ��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCNXCL:
// ��λ�ն˴�����·ר��ģʽ��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCSCTTY:
// ����tty Ϊ�����նˡ�(TIOCNOTTY - ��ֹtty Ϊ�����ն�)��
		return -EINVAL;		/* set controlling term NI *//* ���ÿ����ն�NI */
	case TIOCGPGRP:		// NI - Not Implemented��
// ��ȡָ���ն��豸���̵���id��������֤�û����������ȣ�Ȼ����tty ��pgrp �ֶε��û���������
		verify_area ((void *) arg, 4);
		put_fs_long (tty->pgrp, (unsigned long *) arg);
		return 0;
	case TIOCSPGRP:
// ����ָ���ն��豸���̵���id��
		tty->pgrp = get_fs_long ((unsigned long *) arg);
		return 0;
	case TIOCOUTQ:
// ������������л�δ�ͳ����ַ�����������֤�û����������ȣ�Ȼ���ƶ������ַ������û���
		verify_area ((void *) arg, 4);
		put_fs_long (CHARS (tty->write_q), (unsigned long *) arg);
		return 0;
	case TIOCINQ:
// ������������л�δ��ȡ���ַ�����������֤�û����������ȣ�Ȼ���ƶ������ַ������û���
		verify_area ((void *) arg, 4);
		put_fs_long (CHARS (tty->secondary), (unsigned long *) arg);
		return 0;
	case TIOCSTI:
// ģ���ն����롣��������һ��ָ���ַ���ָ����Ϊ����������װ���ַ������ն��ϼ���ġ��û�����
// �ڸÿ����ն��Ͼ��г����û�Ȩ�޻���ж����Ȩ�ޡ�
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCGWINSZ:
// ��ȡ�ն��豸���ڴ�С��Ϣ���μ�termios.h �е�winsize �ṹ����
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCSWINSZ:
// �����ն��豸���ڴ�С��Ϣ���μ�winsize �ṹ����
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCMGET:
// ����modem ״̬�������ߵĵ�ǰ״̬����λ��־�����μ�termios.h ��185-196 �У���
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCMBIS:
// ���õ���modem ״̬�������ߵ�״̬(true ��false)��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCMBIC:
// ��λ����modem ״̬�������ߵ�״̬��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCMSET:
// ����modem ״̬���ߵ�״̬�����ĳһ����λ��λ����modem ��Ӧ��״̬���߽���Ϊ��Ч��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCGSOFTCAR:
// ��ȡ����ز�����־(1 - ������0 - �ر�)��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	case TIOCSSOFTCAR:
// ��������ز�����־(1 - ������0 - �ر�)��
		return -EINVAL;		/* not implemented *//* δʵ�� */
	default:
		return -EINVAL;
	}
}
