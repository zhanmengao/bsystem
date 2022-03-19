/*
 *  linux/lib/open.c
 *
 *  open()ϵͳ�������ڽ�һ���ļ���ת����һ���ļ��������������óɹ�ʱ�����ص��ļ�������
 * ���ǽ���û�д򿪵���С��ֵ�����������õ��ô���һ���µĴ��ļ����������κ���������
 * ������ִ��exec����ʱ�����µ��ļ���������ʼ�ձ����Ŵ�״̬���ļ��Ķ�дָ�뱻����
 * ���ļ���ʼλ�á�����flag ��0_RDONLY��O_WRONLY��O_RDWR ֮һ���ֱ�����ļ�ֻ���򿪡�ֻ
 * д�򿪺Ͷ�д�򿪷�ʽ������������һЩ��־һ��ʹ�á�(�μ�fs/open.c��138 ��)
 */
#include <set_seg.h>

#define __LIBRARY__
#include <unistd.h>// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
								// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <stdarg.h>// ��׼����ͷ�ļ����Ժ����ʽ������������б���Ҫ˵����-��
								// ����(va_list)��������(va_start, va_arg ��va_end)������
								// vsprintf��vprintf��vfprintf ������

//// ���ļ�������
// �򿪲��п��ܴ���һ���ļ���
// ������filename - �ļ�����flag - �ļ��򿪱�־��...
// ���أ��ļ������������������ó����룬������-1��
int open(const char * filename, int flag, ...)
{
	register int res;
	va_list arg;

// ����va_start()�꺯����ȡ��flag ���������ָ�룬Ȼ�����ϵͳ�ж�int 0x80������open ����
// �ļ��򿪲�����
// %0 - eax(���ص��������������)��%1 - eax(ϵͳ�жϵ��ù��ܺ�__NR_open)��
// %2 - ebx(�ļ���filename)��%3 - ecx(���ļ���־flag)��%4 - edx(��������ļ�����mode)��
	va_start(arg,flag);
	res = va_arg(arg,int);
	_asm{
		mov eax,__NR_open
		mov ebx,filename
		mov ecx,flag
		mov edx,res
		int 0x80
		mov res,eax
	}
/*	__asm__("int $0x80"
		:"=a" (res)
		:"0" (__NR_open),"b" (filename),"c" (flag),
		"d" (va_arg(arg,int)));*/
// ϵͳ�жϵ��÷���ֵ���ڻ����0����ʾ��һ���ļ�����������ֱ�ӷ���֮��
	if (res>=0)
		return res;
// ����˵������ֵС��0�������һ�������롣���øó����벢����-1��
	errno = -res;
	return -1;
}
