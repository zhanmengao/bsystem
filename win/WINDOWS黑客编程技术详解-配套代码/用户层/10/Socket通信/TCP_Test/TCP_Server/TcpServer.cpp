#include "stdafx.h"
#include "TcpServer.h"


// ������׽���
SOCKET g_ServerSocket;
// �ͻ����׽���
SOCKET g_ClientSocket;


// �󶨶˿ڲ�����
BOOL SocketBindAndListen(char *lpszIp, int iPort)
{
	// ��ʼ�� Winsock ��
	WSADATA wsaData = {0};
	::WSAStartup(MAKEWORD(2, 2), &wsaData);

	// ������ʽ�׽���
	g_ServerSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == g_ServerSocket)
	{
		return FALSE;
	}
	// ���÷���˵�ַ�Ͷ˿���Ϣ
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszIp);
	// ��IP�Ͷ˿�
	if (0 != ::bind(g_ServerSocket, (sockaddr *)(&addr), sizeof(addr)))
	{
		return FALSE;
	}
	// ���ü���
	if (0 != ::listen(g_ServerSocket, 1))
	{
		return FALSE;
	}

	// �����������ݶ��߳�
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThreadProc, NULL, NULL, NULL);

	return TRUE;
}


// ��������
void SendMsg(char *pszSend)
{
	// ��������
	::send(g_ClientSocket, pszSend, (1 + ::lstrlen(pszSend)), 0);
	printf("[send]%s\n", pszSend);
}


// ������������ �� ��������
void AcceptRecvMsg()
{
	sockaddr_in addr = { 0 };
	// ע�⣺�ñ�����������Ҳ�����
	int iLen = sizeof(addr);   
	// �������Կͻ��˵���������
	g_ClientSocket = ::accept(g_ServerSocket, (sockaddr *)(&addr), &iLen);
	printf("accept a connection from client!\n");

	char szBuf[MAX_PATH] = { 0 };
	while (TRUE)
	{
		// ��������
		int iRet = ::recv(g_ClientSocket, szBuf, MAX_PATH, 0);
		if (0 >= iRet)
		{
			continue;
		}
		printf("[recv]%s\n", szBuf);
	}
}


// �������ݶ��߳�
UINT RecvThreadProc(LPVOID lpVoid) 
{
	// ������������ �� ��������
	AcceptRecvMsg();
	return 0;
}
