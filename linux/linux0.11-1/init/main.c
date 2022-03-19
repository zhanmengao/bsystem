/* passed
 *  linux/init/main.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#define __LIBRARY__	// ����ñ�����Ϊ�˰���������unistd.h �е���Ƕ���������Ϣ��

#include <unistd.h>
#include <time.h> // ʱ������ͷ�ļ�����������Ҫ������tm �ṹ��һЩ�й�
								// ʱ��ĺ���ԭ�Ρ�

/*
 * ������Ҫ������Щ��Ƕ���- ���ں˿ռ䴴������(forking)������û��дʱ��
 * �ƣ�COPY ON WRITE��!!!ֱ��һ��ִ��execve ���á���Զ�ջ���ܴ������⡣��
 * ��ķ�������fork()����֮����main()ʹ���κζ�ջ����˾Ͳ����к�����
 * ��- ����ζ��fork ҲҪʹ����Ƕ�Ĵ��룬���������ڴ�fork()�˳�ʱ��Ҫʹ�ö�ջ�ˡ�
 *
 * ʵ����ֻ��pause ��fork ��Ҫʹ����Ƕ��ʽ���Ա�֤��main()�в���Ū�Ҷ�ջ��
 * ��������ͬʱ������������һЩ������
 */
static _inline _syscall0(int,fork)// ��unistd.h �е���Ƕ����롣��Ƕ�������ʽ����
								// Linux ��ϵͳ�����ж�0x80�����ж�������ϵͳ���õ�
								// ��ڡ��������ʵ������int fork()��������ϵͳ���á�
								// syscall0 ����������0 ��ʾ�޲�����1 ��ʾ1 ��������
static _inline _syscall0(int,pause)// int pause()ϵͳ���ã���ͣ���̵�ִ�У�ֱ��
								// �յ�һ���źš�
static _inline _syscall1(int,setup,void *,BIOS)// int setup(void * BIOS)ϵͳ���ã�������
												// linux ��ʼ����������������б����ã���
static _inline _syscall0(int,sync)// int sync()ϵͳ���ã������ļ�ϵͳ��

#include <linux/tty.h>	// tty ͷ�ļ����������й�tty_io������ͨ�ŷ����
									// ������������
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����1 ��
									// ��ʼ��������ݡ�����һЩ�Ժ����ʽ������й�������
									// �������úͻ�ȡ��Ƕ��ʽ��ຯ������
#include <linux/head.h>	// head ͷ�ļ��������˶��������ļ򵥽ṹ��
									// �ͼ���ѡ���������
#include <asm/system.h>	// ϵͳͷ�ļ����Ժ����ʽ����������й����û��޸�
									// ������/�ж��ŵȵ�Ƕ��ʽ����ӳ���
#include <asm/io.h>		// io ͷ�ļ����Ժ��Ƕ���������ʽ�����io ��
									// �ڲ����ĺ�����

#include <stddef.h>	// ��׼����ͷ�ļ���������NULL, offsetof(TYPE, MEMBER)��
#include <stdarg.h>	// ��׼����ͷ�ļ����Ժ����ʽ������������б���Ҫ˵
								// ����-������(va_list)��������(va_start, va_arg ��
								// va_end)��vsprintf��vprintf��vfprintf��
#include <fcntl.h>	// �ļ�����ͷ�ļ��������ļ������������Ĳ������Ƴ���
								// ���ŵĶ��塣
#include <sys/types.h>// ����ͷ�ļ��������˻�����ϵͳ�������͡�

#include <linux/fs.h>// �ļ�ϵͳͷ�ļ��������ļ���ṹ
								//��file,buffer_head,m_inode �ȣ���

static char printbuf[1024];

extern int vsprintf();	// �͸�ʽ�������һ�ַ����У���kernel/vsprintf.c����
extern void init(void);	// ����ԭ�Σ���ʼ�����ں��棩��
extern void blk_dev_init(void);// ���豸��ʼ���ӳ���kernel/blk_drv/ll_rw_blk.c��
extern void chr_dev_init(void);// �ַ��豸��ʼ����kernel/chr_drv/tty_io.c��
extern void hd_init(void);// Ӳ�̳�ʼ������kernel/blk_drv/hd.c��
extern void floppy_init(void);// ������ʼ������kernel/blk_drv/floppy.c��
extern void mem_init(long start, long end);// �ڴ�����ʼ����mm/memory.c��
extern long rd_init(long mem_start, int length);//�����̳�ʼ��(kernel/blk_drv/ramdisk.c)
extern long kernel_mktime(struct tm * tm);// �����ں�ʱ�䣨�룩��
extern long startup_time;// �ں�����ʱ�䣨����ʱ�䣩���룩��

/*
 * ������Щ��������setup.s ����������ʱ�����õġ�
 */
#define EXT_MEM_K (*(unsigned short *)0x90002)// 1M �Ժ����չ�ڴ��С��KB����
#define DRIVE_INFO (*(struct drive_info *)0x90080)// Ӳ�̲������ַ��
#define ORIG_ROOT_DEV (*(unsigned short *)0x901FC)// ���ļ�ϵͳ�����豸�š�

/*
 * �ǰ����ǰ���������γ���ܲ�����Ҳ�֪�������ȷ��ʵ�֣����Һ���
 * ���������С�����й���ʵʱʱ�Ӹ�������ϣ����Һܸ���Ȥ����Щ������
 * ̽�����ģ��Լ�����һЩbios ���򣬺ǣ�
 */
// ��κ��ȡCMOS ʵʱʱ����Ϣ��
// 0x70 ��д�˿ںţ�0x80|addr ��Ҫ��ȡ��CMOS �ڴ��ַ��
// 0x71 �Ƕ��˿ںš�
/*
#define CMOS_READ(addr) ({ \
outb_p(0x80|addr,0x70); \
inb_p(0x71); \
})*/
_inline unsigned char CMOS_READ(unsigned char addr)
{
	outb_p(addr,0x70);
	return inb_p(0x71);
}

// ��BCD ��ת�������֡�
#define BCD_TO_BIN(val) ((val)=((val)&15) + ((val)>>4)*10)

// ���ӳ���ȡCMOS ʱ�ӣ������ÿ���ʱ�� startup_time(Ϊ��1970-1-1-0 ʱ�𵽿���ʱ������)��
static void time_init(void)
{
	struct tm time;

	do {// �μ�����CMOS �ڴ��б�
		time.tm_sec = CMOS_READ(0);
		time.tm_min = CMOS_READ(2);
		time.tm_hour = CMOS_READ(4);
		time.tm_mday = CMOS_READ(7);
		time.tm_mon = CMOS_READ(8);
		time.tm_year = CMOS_READ(9);
	} while (time.tm_sec != CMOS_READ(0));
	BCD_TO_BIN(time.tm_sec);
	BCD_TO_BIN(time.tm_min);
	BCD_TO_BIN(time.tm_hour);
	BCD_TO_BIN(time.tm_mday);
	BCD_TO_BIN(time.tm_mon);
	BCD_TO_BIN(time.tm_year);
	time.tm_mon--;
	startup_time = kernel_mktime(&time);
}

static long memory_end = 0;// �������е��ڴ棨�ֽ�������
static long buffer_memory_end = 0;// ���ٻ�����ĩ�˵�ַ��
static long main_memory_start = 0;// ���ڴ棨�����ڷ�ҳ����ʼ��λ�á�

struct drive_info { char dummy[32]; } drive_info;// ���ڴ��Ӳ�̲�������Ϣ��

void main_rename(void)		/* ����ȷʵ��void����û�� */
{			/* ��startup ����(head.s)�о�����������ġ� */
/*
 * ��ʱ�ж��Ա���ֹ�ţ������Ҫ�����ú�ͽ��俪����
 */
	// ������δ������ڱ��棺
	// ���豸�� -> ROOT_DEV�� ���ٻ���ĩ�˵�ַ -> buffer_memory_end��
	// �����ڴ��� -> memory_end�����ڴ濪ʼ��ַ -> main_memory_start��
 	ROOT_DEV = ORIG_ROOT_DEV;
 	drive_info = DRIVE_INFO;
	memory_end = (1<<20) + (EXT_MEM_K<<10);// �ڴ��С=1Mb �ֽ�+��չ�ڴ�(k)*1024 �ֽڡ�
	memory_end &= 0xfffff000;			// ���Բ���4Kb��1 ҳ�����ڴ�����
	if (memory_end > 16*1024*1024)		// ����ڴ泬��16Mb����16Mb �ơ�
		memory_end = 16*1024*1024;
	if (memory_end > 12*1024*1024)		// ����ڴ�>12Mb�������û�����ĩ��=4Mb
		buffer_memory_end = 4*1024*1024;
	else if (memory_end > 6*1024*1024)	// ��������ڴ�>6Mb�������û�����ĩ��=2Mb
		buffer_memory_end = 2*1024*1024;
	else
		buffer_memory_end = 1*1024*1024;// ���������û�����ĩ��=1Mb
	main_memory_start = buffer_memory_end;// ���ڴ���ʼλ��=������ĩ�ˣ�
#ifdef RAMDISK	// ��������������̣������ڴ潫���١�
	main_memory_start += rd_init(main_memory_start, RAMDISK*1024);
#endif
// �������ں˽������з���ĳ�ʼ���������Ķ�ʱ��ø��ŵ��õĳ��������ȥ����ʵ�ڿ�
// ����ȥ�ˣ����ȷ�һ�ţ�����һ����ʼ������-- ���Ǿ���̸֮:)
	mem_init(main_memory_start,memory_end);
	trap_init();	// �����ţ�Ӳ���ж���������ʼ������kernel/traps.c��
	blk_dev_init();	// ���豸��ʼ������kernel/blk_dev/ll_rw_blk.c��
	chr_dev_init();	// �ַ��豸��ʼ������kernel/chr_dev/tty_io.c���գ�Ϊ�Ժ���չ��׼����
	tty_init();		// tty ��ʼ������kernel/chr_dev/tty_io.c��
	time_init();	// ���ÿ�������ʱ�� -> startup_time��
	sched_init();	// ���ȳ����ʼ��(����������0 ��tr, ldtr) ��kernel/sched.c��
	buffer_init(buffer_memory_end);// ��������ʼ�������ڴ�����ȡ���fs/buffer.c��
	hd_init();		// Ӳ�̳�ʼ������kernel/blk_dev/hd.c��
	floppy_init();	// ������ʼ������kernel/blk_dev/floppy.c��
	sti();			// ���г�ʼ�������������ˣ������жϡ�

// �������ͨ���ڶ�ջ�����õĲ����������жϷ���ָ���л�������0��
	move_to_user_mode();	// �Ƶ��û�ģʽ����include/asm/system.h��
	if (!fork()) {		/* we count on this going ok */
		init();
	}
/*
 * ע��!! �����κ�����������'pause()'����ζ�����Ǳ���ȴ��յ�һ���źŲŻ᷵
 * �ؾ�������̬��������0��task0����Ψһ������������μ�'schedule()'������Ϊ��
 * ��0 ���κο���ʱ���ﶼ�ᱻ�����û����������������ʱ����
 * ��˶�������0'pause()'����ζ�����Ƿ������鿴�Ƿ�����������������У����û
 * �еĻ����Ǿͻص����һֱѭ��ִ��'pause()'��
 */
	for(;;) pause();
} // end main


static int printf(const char *fmt, ...)
// ������ʽ����Ϣ���������׼����豸stdout(1)��������ָ��Ļ����ʾ������'*fmt'
// ָ����������õĸ�ʽ���μ����ֱ�׼C �����鼮�����ӳ���������vsprintf ���ʹ
// �õ�һ�����ӡ�
// �ó���ʹ��vsprintf()����ʽ�����ַ�������printbuf ��������Ȼ����write()
// ���������������������׼�豸��1--stdout����
{
	va_list args;
	int i;

	va_start(args, fmt);
	write(1,printbuf,i=vsprintf(printbuf, fmt, args));
	va_end(args);
	return i;
}

static char * argv_rc[] = { "/bin/sh", NULL };// ����ִ�г���ʱ�������ַ������顣
static char * envp_rc[] = { "HOME=/", NULL };// ����ִ�г���ʱ�Ļ����ַ������顣

static char * argv[] = { "-/bin/sh",NULL };		// ͬ�ϡ�
static char * envp[] = { "HOME=/usr/root", NULL };

void init(void)
{
	int pid,i;

// ��ȡӲ�̲���������������Ϣ�����������̺Ͱ�װ���ļ�ϵͳ�豸��
// �ú�������25 ���ϵĺ궨��ģ���Ӧ������sys_setup()����kernel/blk_drv/hd.c��
	setup((void *) &drive_info);

	(void) open("/dev/tty0",O_RDWR,0);	// �ö�д���ʷ�ʽ���豸��/dev/tty0����
										// �����Ӧ�ն˿���̨��
										// ���صľ����0 -- stdin ��׼�����豸��
	(void) dup(0);		// ���ƾ�����������1 ��-- stdout ��׼����豸��
	(void) dup(0);		// ���ƾ�����������2 ��-- stderr ��׼��������豸��
	printf("%d buffers = %d bytes buffer space\n\r",NR_BUFFERS, \
		NR_BUFFERS*BLOCK_SIZE);	// ��ӡ���������������ֽ�����ÿ��1024 �ֽڡ�
	printf("Free mem: %d bytes\n\r",memory_end-main_memory_start);//�����ڴ��ֽ�����

// ����fork()���ڴ���һ���ӽ���(������)�����ڱ��������ӽ��̣�fork()������0 ֵ��
// ����ԭ(������)�������ӽ��̵Ľ��̺š�����if (!(pid=fork())) {...} �����ӽ���ִ�е����ݡ�
// ���ӽ��̹ر��˾��0(stdin)����ֻ����ʽ��/etc/rc �ļ�����ִ��/bin/sh ��������������
// ���������ֱ���argv_rc ��envp_rc ����������μ������������
	if (!(pid=fork())) {
		close(0);
		if (open("/etc/rc",O_RDONLY,0))
			_exit(1);	// ������ļ�ʧ�ܣ����˳�(/lib/_exit.c)��
		execve("/bin/sh",argv_rc,envp_rc);	// װ��/bin/sh ����ִ�С�(/lib/execve.c)
		_exit(2);	// ��execve()ִ��ʧ�����˳�(������2,���ļ���Ŀ¼�����ڡ�)��
	}

// �����Ǹ�����ִ�е���䡣wait()�ǵȴ��ӽ���ֹͣ����ֹ���䷵��ֵӦ���ӽ��̵�
// ���̺�(pid)��������������Ǹ����̵ȴ��ӽ��̵Ľ�����&i �Ǵ�ŷ���״̬��Ϣ��
// λ�á����wait()����ֵ�������ӽ��̺ţ�������ȴ���
	if (pid>0)
		while (pid != wait(&i))
		{	/* nothing */;}

// --
// ���ִ�е����˵���մ������ӽ��̵�ִ����ֹͣ����ֹ�ˡ�����ѭ���������ٴ���
// һ���ӽ��̣������������ʾ����ʼ�����򴴽��ӽ���ʧ�ܡ�����Ϣ������ִ�С���
// �����������ӽ��̹ر�������ǰ�������ľ��(stdin, stdout, stderr)���´���һ��
// �Ự�����ý�����ţ�Ȼ�����´�/dev/tty0 ��Ϊstdin�������Ƴ�stdout ��stderr��
// �ٴ�ִ��ϵͳ���ͳ���/bin/sh�������ִ����ѡ�õĲ����ͻ���������ѡ��һ�ף������棩��
// Ȼ�󸸽����ٴ�����wait()�ȴ�������ӽ�����ֹͣ��ִ�У����ڱ�׼�������ʾ������Ϣ
//		���ӽ���pid ֹͣ�����У���������i����
// Ȼ�����������ȥ�����γɡ�����ѭ����
	while (1) {
		if ((pid=fork())<0) {
			printf("Fork failed in init\r\n");
			continue;
		}
		if (!pid) {
			close(0);close(1);close(2);
			setsid();
			(void) open("/dev/tty0",O_RDWR,0);
			(void) dup(0);
			(void) dup(0);
			_exit(execve("/bin/sh",argv,envp));
		}
		while (1)
			if (pid == wait(&i))
				break;
		printf("\n\rchild %d died with code %04x\n\r",pid,i);
		sync();
	}
	_exit(0);	/* NOTE! _exit, not exit() */
}