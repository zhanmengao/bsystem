// UDP_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UDPTest.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szIp[MAX_PATH] = { 0 };
	int iPort = 0;
	// �������UDP�󶨵�IP�Ͷ˿�
	printf("Input IP and Port:\n");
	scanf("%s%d", szIp, &iPort);
	getchar();

	// �󶨵�ַ
	if (FALSE == Bind(szIp, iPort))
	{
		printf("Bind Error.\n");
	}
	printf("Bind OK.\n");

	// ���뷢������Ŀ��������IP�Ͷ˿�
	printf("Input Dest IP and Dest Port:\n");
	scanf("%s%d", szIp, &iPort);
	getchar();
	// ��������
	char szBuf[MAX_PATH] = {0};
	while (TRUE)
	{
		gets(szBuf);
		SendMsg(szBuf, szIp, iPort);
	}

	return 0;
}

