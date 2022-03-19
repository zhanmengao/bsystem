#include "stdafx.h"
#include "UDPTest.h"


SOCKET g_sock;


// ��IP��ַ�Ͷ˿�
BOOL Bind(char *lpszIp, int iPort)
{
	// ��ʼ�� Winsock ��
	WSADATA wsaData = { 0 };
	::WSAStartup(MAKEWORD(2, 2), &wsaData);

	// �������ݱ��׽���
	g_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == g_sock)
	{
		return FALSE;
	}
	// ���ð�IP��ַ�Ͷ˿���Ϣ
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszIp);
	// ��IP��ַ�Ͷ˿�
	if (0 != bind(g_sock, (sockaddr *)(&addr), sizeof(addr)))
	{
		return FALSE;
	}
	// ����������Ϣ���߳�
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)RecvThreadProc, NULL, NULL, NULL);
	return TRUE;
}


// ���ݷ���
void SendMsg(char *lpszText, char *lpszIp, int iPort)
{
	// ����Ŀ��������IP��ַ�Ͷ˿ڵȵ�ַ��Ϣ
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(iPort);
	addr.sin_addr.S_un.S_addr = ::inet_addr(lpszIp);
	// �������ݵ�Ŀ������
	::sendto(g_sock, lpszText, (1 + ::lstrlen(lpszText)), 0, (sockaddr *)(&addr), sizeof(addr));
	printf("[sendto]%s\n", lpszText);
}


// ���ݽ���
void RecvMsg()
{
	char szBuf[MAX_PATH] = { 0 };
	while (TRUE)
	{
		sockaddr_in addr = { 0 };
		// ע��˴�, �����������Ҳ���������
		int iLen = sizeof(addr);   
		// ��������
		::recvfrom(g_sock, szBuf, MAX_PATH, 0, (sockaddr *)(&addr), &iLen);
		printf("[recvfrom]%s\n", szBuf);
	}
}


// �������ݶ��߳�
UINT RecvThreadProc(LPVOID lpVoid)
{
	// ��������
	RecvMsg();
	return 0;
}