// HTTPS_Download_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HttpsInet.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szHttpsDownloadUrl[] = "https://download.microsoft.com/download/0/2/3/02389126-40A7-46FD-9D83-802454852703/vc_mbcsmfc.exe";
	BYTE *pHttpsDownloadData = NULL;
	DWORD dwHttpsDownloadDataSize = 0;
	// HTTPS���� 
	if (FALSE == Https_Download(szHttpsDownloadUrl, &pHttpsDownloadData, &dwHttpsDownloadDataSize))
	{
		return 1;
	}
	// �����ݱ�����ļ�
	Https_SaveToFile("https_downloadsavefile.exe", pHttpsDownloadData, dwHttpsDownloadDataSize);
	// �ͷ��ڴ�
	delete []pHttpsDownloadData;
	pHttpsDownloadData = NULL;

	system("pause");
	return 0;
}

