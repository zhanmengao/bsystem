#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_


#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")


// �󶨶˿ڲ�����
BOOL SocketBindAndListen(char *lpszIp, int iPort);
// ��������
void SendMsg(char *pszSend);
// �����������ݶ��߳�
void AcceptRecvMsg();
// �������ݶ��߳�
UINT RecvThreadProc(LPVOID lpVoid);


#endif