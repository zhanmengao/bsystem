// TCP_Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TcpClient.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ���ӵ�������
	if (FALSE == Connection("127.0.0.1", 12345))
	{
		printf("Connection Error.\n");
	}
	printf("Connection OK.\n");

	// ������Ϣ
	char szSendBuf[MAX_PATH] = { 0 };
	while (TRUE)
	{
		gets(szSendBuf);
		// ��������
		SendMsg(szSendBuf);
	}

	return 0;
}

