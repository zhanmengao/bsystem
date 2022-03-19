/* passed
* linux/kernel/math/math_emulate.c
*
* (C) 1991 Linus Torvalds
*/
/*
ע�⣺signal.c��fork.c�ļ��ı���ѡ���ڲ�����vc�����Ż�ѡ��/Og����Ϊ�������ļ�
	�ڵĺ��������ڰ����˺������ص�ַ�����ݡ��������/Ogѡ��������ͻ�����Ϊ
	��Щ��������ʹ�ú�ռ�ø��ڴ棬���º�������ʱ����
	math/math_emulate.c����ҲӦ������������������û�а�eip�Ȳ����Ż���:)
*/
#include <set_seg.h>

/*
* This directory should contain the math-emulation code.
* Currently only results in a signal.
*/
/*
* ��Ŀ¼��Ӧ�ð�����ѧ������롣Ŀǰ������һ���źš�
*/

#include <signal.h>		// �ź�ͷ�ļ��������źŷ��ų������źŽṹ�Լ��źŲ�������ԭ�͡�

#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

//// Э���������溯����
// �жϴ��������õ�C �������μ�(kernel/math/system_call.s��169 ��)��
void
math_emulate (long edi, long esi, long ebp, long sys_call_ret,
	      long eax, long ebx, long ecx, long edx,
	      unsigned short fs, unsigned short es, unsigned short ds,
	      unsigned long eip, unsigned short cs, unsigned long eflags,
	      unsigned short ss, unsigned long esp)
{
	unsigned char first, second;

/* 0x0007 means user code space */
/* 0x0007 ��ʾ�û�����ռ� */
// ѡ���0x000F ��ʾ�ھֲ���������������������ֵ=1��������ռ䡣����μĴ���cs ������0x000F
// ���ʾcs һ�����ں˴���ѡ����������ں˴���ռ䣬�������ʾ��ʱ��cs:eip ֵ������ʾ��Ϣ
// ���ں�����Ҫ��ѧ���桱��Ȼ���������״̬��
	if (cs != 0x000F)
	{
		printk ("math_emulate: %04x:%08x\n\r", cs, eip);
		panic ("Math emulation needed in kernel");
	}
// ȡ�û���������ջ����first ��second����ʾ��Щ���ݣ������������ø����쳣�ź�SIGFPE��
	first = get_fs_byte ((char *) ((*&eip)++));
	second = get_fs_byte ((char *) ((*&eip)++));
	printk ("%04x:%08x %02x %02x\n\r", cs, eip - 2, first, second);
	current->signal |= 1 << (SIGFPE - 1);
}

//// Э����������������
// �жϴ��������õ�C �������μ�(kernel/math/system_call.s��145 ��)��
void
math_error (void)
{
// Э������ָ�(�Էǵȴ���ʽ)��������쳣��־��æ��־��״̬��λ7��
//  __asm__ ("fnclex");
	_asm fnclex;
// ����ϸ�����ʹ�ù�Э�������������ϸ�������Э�������쳣�źš�
	if (last_task_used_math)
		last_task_used_math->signal |= 1 << (SIGFPE - 1);
}
