#ifndef _UTIME_H
#define _UTIME_H

#include <sys/types.h>		/* ��֪�� - ��Ӧ��������������.. */

struct utimbuf
{
  time_t actime;		// �ļ�����ʱ�䡣��1970.1.1:0:0:0 ��ʼ��������
  time_t modtime;		// �ļ��޸�ʱ�䡣��1970.1.1:0:0:0 ��ʼ��������
};

// �����ļ����ʺ��޸�ʱ�亯����
extern int utime (const char *filename, struct utimbuf *times);

#endif
