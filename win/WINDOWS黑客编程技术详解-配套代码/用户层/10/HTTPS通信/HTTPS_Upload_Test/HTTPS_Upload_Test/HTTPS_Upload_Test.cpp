// HTTPS_Upload_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HttpsInet.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szHttpsUploadUrl[] = "https://192.168.28.137/mytest1.asp?file=520.zip";
	char szHttpsUploadFileName[] = "C:\\Users\\DemonGan\\Desktop\\520.zip";
	BYTE *pHttpsUploadData = NULL;
	DWORD dwHttpsUploadDataSize = 0;
	DWORD dwRets = 0;
	// ���ļ�
	HANDLE hFiles = ::CreateFile(szHttpsUploadFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFiles)
	{
		return 1;
	}
	// ��ȡ�ļ���С
	dwHttpsUploadDataSize = ::GetFileSize(hFiles, NULL);
	// ��ȡ�ļ�����
	pHttpsUploadData = new BYTE[dwHttpsUploadDataSize];
	::ReadFile(hFiles, pHttpsUploadData, dwHttpsUploadDataSize, &dwRets, NULL);
	dwHttpsUploadDataSize = dwRets;
	// �ϴ�����
	if (FALSE == Https_Upload(szHttpsUploadUrl, pHttpsUploadData, dwHttpsUploadDataSize))
	{
		return 2;
	}
	// �ͷ��ڴ�
	delete []pHttpsUploadData;
	pHttpsUploadData = NULL;
	::CloseHandle(hFiles);

	system("pause");
	return 0;
}

