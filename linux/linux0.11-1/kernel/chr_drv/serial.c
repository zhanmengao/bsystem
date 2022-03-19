/* passed
* linux/kernel/serial.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/*
* serial.c
*
* This module implements the rs232 io functions
* void rs_write(struct tty_struct * queue);
* void rs_init(void);
* and all interrupts pertaining to serial IO.
*/
/*
* serial.c
* �ó�������ʵ��rs232 �������������
* void rs_write(struct tty_struct *queue);
* void rs_init(void);
* �Լ��봫��IO �й�ϵ�������жϴ������
*/

#include <linux/tty.h>		// tty ͷ�ļ����������й�tty_io������ͨ�ŷ���Ĳ�����������
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <asm/system.h>		// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <asm/io.h>		// io ͷ�ļ�������Ӳ���˿�����/���������䡣

#define WAKEUP_CHARS (TTY_BUF_SIZE/4)	// ��д�����к���WAKEUP_CHARS ���ַ�ʱ���Ϳ�ʼ���͡�

extern void rs1_interrupt (void);	// ���п�1 ���жϴ������(rs_io.s, 34)��
extern void rs2_interrupt (void);	// ���п�2 ���жϴ������(rs_io.s, 38)��

//// ��ʼ�����ж˿�
// port: ����1 - 0x3F8������2 - 0x2F8��
static void init (int port)
{
	outb_p (0x80, port + 3);	/* set DLAB of line control reg */
/* ������·���ƼĴ�����DLAB λ(λ7) */
	outb_p (0x30, port);		/* LS of divisor (48 -> 2400 bps */
/* ���Ͳ��������ӵ��ֽڣ�0x30->2400bps */
	outb_p (0x00, port + 1);	/* MS of divisor */
/* ���Ͳ��������Ӹ��ֽڣ�0x00 */
	outb_p (0x03, port + 3);	/* reset DLAB */
/* ��λDLAB λ������λΪ8 λ */
	outb_p (0x0b, port + 4);	/* set DTR,RTS, OUT_2 */
/* ����DTR��RTS�������û����2 */
	outb_p (0x0d, port + 1);	/* enable all intrs but writes */
/* ����д(д���ֿ�)���⣬���������ж�Դ�ж� */
	(void) inb (port);		/* read data port to reset things (?) */
/* �����ݿڣ��Խ��и�λ����(?) */
}

//// ��ʼ�������жϳ���ʹ��нӿڡ�
void
rs_init (void)
{
	set_intr_gate (0x24, rs1_interrupt);	// ���ô��п�1 ���ж�������(Ӳ��IRQ4 �ź�)��
	set_intr_gate (0x23, rs2_interrupt);	// ���ô��п�2 ���ж�������(Ӳ��IRQ3 �ź�)��
	init (tty_table[1].read_q.data);	// ��ʼ�����п�1(.data �Ƕ˿ں�)��
	init (tty_table[2].read_q.data);	// ��ʼ�����п�2��
	outb (inb_p (0x21) & 0xE7, 0x21);	// ������8259A оƬ��IRQ3��IRQ4 �ж��ź�����
}

/*
* ��tty_write()�ѽ����ݷ������(д)����ʱ�����������ӳ��򡣱�������
* ���д�����Ƿ�Ϊ�գ�����Ӧ�����жϼĴ�����
*/
//// �������ݷ��������
// ʵ����ֻ�ǿ������з��ͱ��ּĴ����ѿ��жϱ�־����UART �����ݷ��ͳ�ȥ�������ж��źš�
void rs_write (struct tty_struct *tty)
{
	cli ();			// ���жϡ�
// ���д���в��գ����0x3f9(��0x2f9) ���ȶ�ȡ�ж�����Ĵ������ݣ����Ϸ��ͱ��ּĴ���
// �ж������־��λ1������д�ظüĴ�����
	if (!EMPTY (tty->write_q))
		outb (inb_p (tty->write_q.data + 1) | 0x02, tty->write_q.data + 1);
	sti ();			// ���жϡ�
}
