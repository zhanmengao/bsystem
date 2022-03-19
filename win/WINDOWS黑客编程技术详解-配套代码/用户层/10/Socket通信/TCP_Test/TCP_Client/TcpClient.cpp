#include "stdafx.h"
#include "TcpClient.h"


// �ͻ����׽���
SOCKET g_ClientSocket;


// ���ӵ�������
BOOL Connection(char *lpszServerIp, int iServerPort)
{
	// ��ʼ�� Winsock ��
	WSADATA wsaData = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsaData);
	// ������ʽ�׽���
	g_ClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == g_ClientSocket)
	{
		return FALSE;
	}
	// ���÷���˵�ַ�Ͷ˿���Ϣ
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iServerPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszServerIp);
	// ���ӵ�������
	if (0 != ::connect(g_ClientSocket, (sockaddr *)(&addr), sizeof(addr)))
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


// ��������
void RecvMsg()
{
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
	RecvMsg();
	return 0;
}
