// FTP_Upload_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WinInet_Ftp.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if (FALSE == FTPUpload("ftp://admin:123456789@192.168.0.1/myftpuploadtest.txt", (BYTE *)"Hello Wolrd", 12))
	{
		printf("FTP Upload Error.\n");
	}

	printf("FTP Upload OK.\n");
	system("pause");
	return 0;
}

