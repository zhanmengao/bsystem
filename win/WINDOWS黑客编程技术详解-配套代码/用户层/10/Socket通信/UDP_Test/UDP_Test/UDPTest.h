#ifndef _UDP_TEST_H_
#define _UDP_TEST_H_


#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")


// ��IP��ַ�Ͷ˿�
BOOL Bind(char *lpszIp, int iPort);
// ���ݷ���
void SendMsg(char *lpszText, char *lpszIp, int iPort);
// ���ݽ���
void RecvMsg();
// �������ݶ��߳�
UINT RecvThreadProc(LPVOID lpVoid);


#endif