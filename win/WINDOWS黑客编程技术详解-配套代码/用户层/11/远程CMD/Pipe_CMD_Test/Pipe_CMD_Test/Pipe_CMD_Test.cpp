// Pipe_CMD_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "PipeCmd.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szCmd[] = "ping 127.0.0.1";
	char szResultBuffer[512] = {0};
	DWORD dwResultBufferSize = 512;

	// ִ�� cmd ����, ����ȡִ�н������
	if (FALSE == PipeCmd(szCmd, szResultBuffer, dwResultBufferSize))
	{
		printf("pipe cmd error.\n");
	}
	else
	{
		printf("CMDִ�н��Ϊ:\n%s\n", szResultBuffer);
	}

	system("pause");
	return 0;
}

