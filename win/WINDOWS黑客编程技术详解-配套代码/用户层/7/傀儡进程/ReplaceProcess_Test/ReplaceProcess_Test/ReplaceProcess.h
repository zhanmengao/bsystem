#ifndef _REPLACE_PROCESS_H_
#define _REPLACE_PROCESS_H_


#include <Windows.h>


// �������̲��滻�����ڴ�����, ����ִ��˳��
BOOL ReplaceProcess(char *pszFilePath, PVOID pReplaceData, DWORD dwReplaceDataSize, DWORD dwRunOffset);


#endif