// TCP_Server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpServer.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// �����׽��ֲ��󶨵�ַ�˿ڽ��м���
	if (FALSE == SocketBindAndListen("127.0.0.1", 12345))
	{
		printf("SocketBindAndListen Error.\n");
	}
	printf("SocketBindAndListen OK.\n");

	// ������Ϣ
	char szSendBuf[MAX_PATH] = {0};
	while (TRUE)
	{
		gets(szSendBuf);
		// ��������
		SendMsg(szSendBuf);
	}

	return 0;
}

