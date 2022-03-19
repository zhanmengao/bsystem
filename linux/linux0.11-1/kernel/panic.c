/* passed
* linux/kernel/panic.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/*
* This function is used through-out the kernel (includeinh mm and fs)
* to indicate a major problem.
*/
/*
* �ú����������ں���ʹ�ã������� ͷ�ļ�*.h, �ڴ�������mm ���ļ�ϵͳfs �У���
* ����ָ����Ҫ�ĳ������⡣
*/
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣

void sys_sync (void);		/* ʵ����������int (fs/buffer.c,44) */

// �ú���������ʾ�ں��г��ֵ��ش������Ϣ���������ļ�ϵͳͬ��������Ȼ�������ѭ�� -- ������
// �����ǰ����������0 �Ļ�����˵���ǽ�������������һ�û�������ļ�ϵͳͬ��������
void panic (const char *s)
{
	printk ("Kernel panic: %s\n\r", s);
	if (current == task[0])
		printk ("In swapper task - not syncing\n\r");
	else
		sys_sync ();
	for (;;);
}
