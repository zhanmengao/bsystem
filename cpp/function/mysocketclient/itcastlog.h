
//written by wangbaoming1999@163.com
//20140323
//itcastlog.h ��־ͷ�ļ�


#ifndef _ITCAST_LOG_H_
#define _ITCAST_LOG_H_

/*
#define IC_NO_LOG_LEVEL			0
#define IC_DEBUG_LEVEL			1
#define IC_INFO_LEVEL			2
#define IC_WARNING_LEVEL		3
#define IC_ERROR_LEVEL			4;
*/

/************************************************************************/
/* 
const char *file���ļ�����
int line���ļ��к�
int level�����󼶱�
		0 -- û����־
		1 -- debug����
		2 -- info����
		3 -- warning����
		4 -- err����
int status��������
const char *fmt���ɱ����
*/
/************************************************************************/
//ʵ��ʹ�õ�Level
extern int  LogLevel[5];
void ITCAST_LOG(const char *file, int line, int level, int status, const char *fmt, ...);

#endif
