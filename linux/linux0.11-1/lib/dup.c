/*
 *  linux/lib/dup.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#define __LIBRARY__
// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <unistd.h>

//// �����ļ�������������
// ����õ��ú꺯����Ӧ��int dup(int fd)��ֱ�ӵ�����ϵͳ�ж�int 0x80��������__NR_dup��
// ����fd ���ļ���������
_syscall1(int,dup,int,fd)
