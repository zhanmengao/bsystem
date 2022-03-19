/*
 *  linux/lib/write.c
 *
 *  (C) 1991  Linus Torvalds
 * 
 *     �ó����а���һ�����ļ�������д��������write()���ú������ļ�������ָ����
 * �ļ�д��count �ֽڵ����ݵ�������buf �С�
 */
#include <set_seg.h>

#define __LIBRARY__
// Linux ��׼ͷ�ļ��������˸��ַ��ų��������ͣ��������˸��ֺ�����
// �綨����__LIBRARY__���򻹰���ϵͳ���úź���Ƕ���_syscall0()�ȡ�
#include <unistd.h>

//// д�ļ�ϵͳ���ú�����
// �ú�ṹ��Ӧ�ں�����int write(int fd, const char * buf, off_t count)
// ������fd - �ļ���������buf - д������ָ�룻count - д�ֽ�����
// ���أ��ɹ�ʱ����д����ֽ���(0 ��ʾд��0 �ֽ�)������ʱ������-1�����������˳���š�
_syscall3(int,write,int,fd,const char *,buf,off_t,count)
