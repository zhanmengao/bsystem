/*
 *  linux/lib/_exit.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#define __LIBRARY__					// ����һ�����ų�����������˵����

// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <unistd.h>		

//// �ں�ʹ�õĳ���(�˳�)��ֹ������
// ֱ�ӵ���ϵͳ�ж�int 0x80�����ܺ�__NR_exit��
// ������exit_code - �˳��롣
//volatile 
void _exit(int exit_code)
{
	// %0 - eax(ϵͳ���ú�__NR_exit)��%1 - ebx(�˳���exit_code)��
	//__asm__("int $0x80"::"a" (__NR_exit),"b" (exit_code));
	_asm mov eax,__NR_exit
	_asm mov ebx,exit_code
	_asm int 0x80
}
