// SHA256_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SHA256Test.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ��ʽһ �����ļ�����
	string sha2561 = CalSHA256_ByFile("520.exe");
	printf("sha2561=%s\n", sha2561.c_str());
	// ��ʽ�� �����ļ���������
	HANDLE hFile2 = ::CreateFile("520.exe", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
	DWORD dwFileSize2 = ::GetFileSize(hFile2, NULL);
	BYTE *pData2 = new BYTE[dwFileSize2];
	::ReadFile(hFile2, pData2, dwFileSize2, NULL, NULL);
	string sha2562 = CalSHA256_ByMem(pData2, dwFileSize2);
	printf("sha2562=%s\n", sha2562.c_str());

	system("pause");
	return 0;
}

