/*
 *  linux/lib/close.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#define __LIBRARY__
// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <unistd.h>

// �ر��ļ�������
// ����õ��ú꺯����Ӧ��int close(int fd)��ֱ�ӵ�����ϵͳ�ж�int 0x80��������__NR_close��
// ����fd ���ļ���������
_syscall1(int,close,int,fd)
