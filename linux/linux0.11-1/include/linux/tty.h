/*
* 'tty.h'�ж�����tty_io.c ����ʹ�õ�ĳЩ�ṹ������һЩ���塣
*
* ע�⣡���޸�����Ķ���ʱ��һ��Ҫ���rs_io.s ��con_io.s �����в���������⡣
* ��ϵͳ����Щ������ֱ��д�ڳ����еģ���Ҫ��һЩtty_queue �е�ƫ��ֵ����
*/
#ifndef _TTY_H
#define _TTY_H

#include <termios.h>		// �ն������������ͷ�ļ�����Ҫ��������첽ͨ�ſڵ��ն˽ӿڡ�

#define TTY_BUF_SIZE 1024	// tty ��������С��

// tty �ȴ��������ݽṹ��
struct tty_queue
{
  unsigned long data;		// �ȴ����л������е�ǰ����ָ���ַ���[??]����
// ���ڴ����նˣ����Ŵ��ж˿ڵ�ַ��
  unsigned long head;		// ������������ͷָ�롣
  unsigned long tail;		// ������������βָ�롣
  struct task_struct *proc_list;	// �ȴ������б�
  char buf[TTY_BUF_SIZE];	// ���еĻ�������
};

// ���¶�����tty �ȴ������л����������꺯������tail ��ǰ��head �ں󣩡�
// a ������ָ��ǰ��1 �ֽڣ���ѭ����
#define INC(a) ((a) = ((a)+1) & (TTY_BUF_SIZE-1))
// a ������ָ�����1 �ֽڣ���ѭ����
#define DEC(a) ((a) = ((a)-1) & (TTY_BUF_SIZE-1))
// ���ָ�����еĻ�������
#define EMPTY(a) ((a).head == (a).tail)
// ���������ɴ���ַ��ĳ��ȣ����������ȣ���
#define LEFT(a) (((a).tail-(a).head-1)&(TTY_BUF_SIZE-1))
// �����������һ��λ�á�
#define LAST(a) ((a).buf[(TTY_BUF_SIZE-1)&((a).head-1)])
// �������������Ϊ1 �Ļ�����
#define FULL(a) (!LEFT(a))
// ���������Ѵ���ַ��ĳ��ȡ�
#define CHARS(a) (((a).head-(a).tail)&(TTY_BUF_SIZE-1))
// ��queue �����������ȡһ�ַ�(��tail ��������tail+=1)��
#define GETCH(queue,c) \
(void)(c=(queue).buf[(queue).tail],INC((queue).tail))
//(void)({c=(queue).buf[(queue).tail];INC((queue).tail);})
// ��queue ����������з���һ�ַ�����head ��������head+=1����
#define PUTCH(c,queue) \
(void)( (queue).buf[(queue).head]=(c), INC((queue).head) )
//(void)({(queue).buf[(queue).head]=(c);INC((queue).head);})

// �ж��ն˼����ַ����͡�
#define INTR_CHAR(tty) ((tty)->termios.c_cc[VINTR])	// �жϷ���
#define QUIT_CHAR(tty) ((tty)->termios.c_cc[VQUIT])	// �˳�����
#define ERASE_CHAR(tty) ((tty)->termios.c_cc[VERASE])	// ��������
#define KILL_CHAR(tty) ((tty)->termios.c_cc[VKILL])	// ��ֹ����
#define EOF_CHAR(tty) ((tty)->termios.c_cc[VEOF])	// �ļ���������
#define START_CHAR(tty) ((tty)->termios.c_cc[VSTART])	// ��ʼ����
#define STOP_CHAR(tty) ((tty)->termios.c_cc[VSTOP])	// ��������
#define SUSPEND_CHAR(tty) ((tty)->termios.c_cc[VSUSP])	// �������

// tty ���ݽṹ��
struct tty_struct
{
  struct termios termios;	// �ն�io ���ԺͿ����ַ����ݽṹ��
  int pgrp;			// ���������顣
  int stopped;			// ֹͣ��־��
  void (*write) (struct tty_struct * tty);	// tty д����ָ�롣
  struct tty_queue read_q;	// tty �����С�
  struct tty_queue write_q;	// tty д���С�
  struct tty_queue secondary;	// tty ��������(��Ź淶ģʽ�ַ�����)��
};				// �ɳ�Ϊ�淶(��)ģʽ���С�

extern struct tty_struct tty_table[];	// tty �ṹ���顣

/* intr=^C quit=^| erase=del kill=^U
eof=^D vtime=\0 vmin=\1 sxtc=\0
start=^Q stop=^S susp=^Z eol=\0
reprint=^R discard=^U werase=^W lnext=^V
eol2=\0
*/
/* �ж�intr=^C �˳�quit=^| ɾ��erase=del ��ֹkill=^U
* �ļ�����eof=^D vtime=\0 vmin=\1 sxtc=\0
* ��ʼstart=^Q ֹͣstop=^S ����susp=^Z �н���eol=\0
* ����reprint=^R ����discard=^U werase=^W lnext=^V
* �н���eol2=\0
*/
// �����ַ���Ӧ��ASCII ��ֵ��[8 ����]
#define INIT_C_CC "\003\034\177\025\004\0\1\0\021\023\032\0\022\017\027\026\0"

void rs_init (void);		// �첽����ͨ�ų�ʼ����(kernel/chr_drv/serial.c, 37)
void con_init (void);		// �����ն˳�ʼ���� (kernel/chr_drv/console.c, 617)
void tty_init (void);		// tty ��ʼ���� (kernel/chr_drv/tty_io.c, 105)

int tty_read (unsigned c, char *buf, int n);	// (kernel/chr_drv/tty_io.c, 230)
int tty_write (unsigned c, char *buf, int n);	// (kernel/chr_drv/tty_io.c, 290)

void rs_write (struct tty_struct *tty);	// (kernel/chr_drv/serial.c, 53)
void con_write (struct tty_struct *tty);	// (kernel/chr_drv/console.c, 445)

void copy_to_cooked (struct tty_struct *tty);	// (kernel/chr_drv/tty_io.c, 145)

#endif
