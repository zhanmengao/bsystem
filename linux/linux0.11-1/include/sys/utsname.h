#ifndef _SYS_UTSNAME_H
#define _SYS_UTSNAME_H

#include <sys/types.h>		// ����ͷ�ļ��������˻�����ϵͳ�������͡�

struct utsname
{
  char sysname[9];		// ���汾����ϵͳ�����ơ�
  char nodename[9];		// ��ʵ����ص������нڵ����ơ�
  char release[9];		// ��ʵ�ֵĵ�ǰ���м���
  char version[9];		// ���η��еİ汾����
  char machine[9];		// ϵͳ���е�Ӳ���������ơ�
};

extern int uname (struct utsname *utsbuf);

#endif
