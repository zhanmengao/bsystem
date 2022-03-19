/* passed
 *  linux/fs/char_dev.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <errno.h>
// ����ͷ�ļ��������˻�����ϵͳ�������͡�
#include <sys/types.h>

// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/sched.h>
// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/kernel.h>

// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <asm/segment.h>
// io ͷ�ļ�������Ӳ���˿�����/���������䡣
#include <asm/io.h>

extern int tty_read(unsigned minor,char * buf,int count);// �ն˶���
extern int tty_write(unsigned minor,char * buf,int count);// �ն�д��

// �����ַ��豸��д����ָ�����͡�
typedef (*crw_ptr)(int rw,unsigned minor,char * buf,int count,off_t * pos);

//// �����ն˶�д����������
// ������rw - ��д���minor - �ն����豸�ţ�buf - ��������cout - ��д�ֽ�����
//       pos - ��д������ǰָ�룬�����ն˲�������ָ�����á�
// ���أ�ʵ�ʶ�д���ֽ�����
static int rw_ttyx(int rw,unsigned minor,char * buf,int count,off_t * pos)
{
	return ((rw==READ)?tty_read(minor,buf,count):
		tty_write(minor,buf,count));
}

//// �ն˶�д����������
// ͬ��rw_ttyx()��ֻ�������˶Խ����Ƿ��п����ն˵ļ�⡣
static int rw_tty(int rw,unsigned minor,char * buf,int count, off_t * pos)
{
	// ������û�ж�Ӧ�Ŀ����նˣ��򷵻س���š�
	if (current->tty<0)
		return -EPERM;
	// ��������ն˶�д����rw_ttyx()��������ʵ�ʶ�д�ֽ�����
	return rw_ttyx(rw,current->tty,buf,count,pos);
}

//// �ڴ����ݶ�д��δʵ�֡�
static int rw_ram(int rw,char * buf, int count, off_t *pos)
{
	return -EIO;
}

//// �ڴ����ݶ�д����������δʵ�֡�
static int rw_mem(int rw,char * buf, int count, off_t * pos)
{
	return -EIO;
}

//// �ں���������д������δʵ�֡�
static int rw_kmem(int rw,char * buf, int count, off_t * pos)
{
	return -EIO;
}

// �˿ڶ�д����������
// ������rw - ��д���buf - ��������cout - ��д�ֽ�����pos - �˿ڵ�ַ��
// ���أ�ʵ�ʶ�д���ֽ�����
static int rw_port(int rw,char * buf, int count, off_t * pos)
{
	int i=*pos;

// ������Ҫ���д���ֽ��������Ҷ˿ڵ�ַС��64k ʱ��ѭ��ִ�е����ֽڵĶ�д������
	while (count-->0 && i<65536) {
// ���Ƕ������Ӷ˿�i �ж�ȡһ�ֽ����ݲ��ŵ��û��������С�
		if (rw==READ)
			put_fs_byte(inb(i),buf++);
// ����д�������û����ݻ�������ȡһ�ֽ�������˿�i��
		else
			outb(get_fs_byte(buf++),i);
// ǰ��һ���˿ڡ�[??]
		i++;
	}
// �����/д���ֽ���������Ӧ������дָ�롣
	i -= *pos;
	*pos += i;
// ���ض�/д���ֽ�����
	return i;
}

//// �ڴ��д����������
static int rw_memory(int rw, unsigned minor, char * buf, int count, off_t * pos)
{
// �����ڴ��豸���豸�ţ��ֱ���ò�ͬ���ڴ��д������
	switch(minor) {
		case 0:
			return rw_ram(rw,buf,count,pos);
		case 1:
			return rw_mem(rw,buf,count,pos);
		case 2:
			return rw_kmem(rw,buf,count,pos);
		case 3:
			return (rw==READ)?0:count;	/* rw_null */
		case 4:
			return rw_port(rw,buf,count,pos);
		default:
			return -EIO;
	}
}

// ����ϵͳ���豸������
#define NRDEVS ((sizeof (crw_table))/(sizeof (crw_ptr)))

// �ַ��豸��д����ָ���
static crw_ptr crw_table[]={
	NULL,		/* ���豸(���豸) */
	rw_memory,	/* /dev/mem �� */
	NULL,		/* /dev/fd ���� */
	NULL,		/* /dev/hd Ӳ�� */
	rw_ttyx,	/* /dev/ttyx �����ն� */
	rw_tty,		/* /dev/tty �ն� */
	NULL,		/* /dev/lp ��ӡ�� */
	NULL};		/* δ�����ܵ� */

//// �ַ��豸��д����������
// ������rw - ��д���dev - �豸�ţ�buf - ��������count - ��д�ֽ�����pos -��дָ�롣
// ���أ�ʵ�ʶ�/д�ֽ�����
int rw_char(int rw,int dev, char * buf, int count, off_t * pos)
{
	crw_ptr call_addr;

// ����豸�ų���ϵͳ�豸�����򷵻س����롣
	if (MAJOR(dev)>=NRDEVS)
		return -ENODEV;
// �����豸û�ж�Ӧ�Ķ�/д�������򷵻س����롣
	if (!(call_addr=crw_table[MAJOR(dev)]))
		return -ENODEV;
// ���ö�Ӧ�豸�Ķ�д����������������ʵ�ʶ�/д���ֽ�����
	return call_addr(rw,MINOR(dev),buf,count,pos);
}
