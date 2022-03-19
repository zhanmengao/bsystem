// HTTP_Upload_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HttpInet.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szHttpUploadUrl[] = "http://192.168.28.137/mytest1.asp?file=myyyyytestupload1";
	char szHttpUploadFileName[] = "C:\\Users\\DemonGan\\Desktop\\520.zip";						
	BYTE *pHttpUploadData = NULL;
	DWORD dwHttpUploadDataSize = 0;
	DWORD dwRet = 0;
	// ���ļ�
	HANDLE hFile = ::CreateFile(szHttpUploadFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Http_ShowError("CreateFile");
		return 5;
	}
	// ��ȡ�ļ���С
	dwHttpUploadDataSize = ::GetFileSize(hFile, NULL);
	// ��ȡ�ļ�����
	pHttpUploadData = new BYTE[dwHttpUploadDataSize];
	::ReadFile(hFile, pHttpUploadData, dwHttpUploadDataSize, &dwRet, NULL);
	dwHttpUploadDataSize = dwRet;
	// �ϴ�����
	if (FALSE == Http_Upload(szHttpUploadUrl, pHttpUploadData, dwHttpUploadDataSize))
	{
		return 1;
	}
	// �ͷ��ڴ�
	delete []pHttpUploadData;
	pHttpUploadData = NULL;
	::CloseHandle(hFile);

	system("pause");
	return 0;
}

