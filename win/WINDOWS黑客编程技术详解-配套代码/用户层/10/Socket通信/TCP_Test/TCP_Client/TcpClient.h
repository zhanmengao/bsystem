#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_


#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")


// ���ӵ�������
BOOL Connection(char *lpszServerIp, int iServerPort);
// ��������
void SendMsg(char *pszSend);
// �����������ݶ��߳�
void RecvMsg();
// �������ݶ��߳�
UINT RecvThreadProc(LPVOID lpVoid);


#endif