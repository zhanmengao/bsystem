// FTP_Download_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WinInet_Ftp.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BYTE *pDownloadData = NULL;
	DWORD dwDownloadDataSize = 0;
	// ����
	if (FALSE == FTPDownload("ftp://admin:123456789@192.168.0.1/Flower520.zip", &pDownloadData, &dwDownloadDataSize))
	{
		printf("FTP Download Error!\n");
	}
	// �����ݱ���Ϊ�ļ�
	Ftp_SaveToFile("myftpdownloadtest.zip", pDownloadData, dwDownloadDataSize);
	// �ͷ��ڴ�
	delete []pDownloadData;
	pDownloadData = NULL;
	printf("FTP Download OK.\n");
	system("pause");
	return 0;
}

