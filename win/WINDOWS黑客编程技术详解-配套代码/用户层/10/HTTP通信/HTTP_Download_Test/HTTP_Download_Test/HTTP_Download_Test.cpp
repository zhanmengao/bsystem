// HTTP_Download_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HttpInet.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szHttpDownloadUrl[] = "http://www.demongan.com/source/ccc/dasanxia/520.zip";
	BYTE *pHttpDownloadData = NULL;
	DWORD dwHttpDownloadDataSize = 0;
	// HTTP���� 
	if (FALSE == Http_Download(szHttpDownloadUrl, &pHttpDownloadData, &dwHttpDownloadDataSize))
	{
		return 1;
	}
	// ���������ݱ�����ļ�
	Http_SaveToFile("http_downloadsavefile.zip", pHttpDownloadData, dwHttpDownloadDataSize);
	// �ͷ��ڴ�
	delete []pHttpDownloadData;
	pHttpDownloadData = NULL;

	system("pause");
	return 0;
}

