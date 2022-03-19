// RunExeInMem_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MmLoadExe.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szFileName[] = "KuaiZip_Setup_2.8.28.8.exe";

	// ��EXE�ļ�����ȡEXE�ļ���С
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ShowError("CreateFile");
		return 1;
	}
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���붯̬�ڴ沢��ȡDLL���ڴ���
	BYTE *pData = new BYTE[dwFileSize];
	if (NULL == pData)
	{
		ShowError("new");
		return 2;
	}
	DWORD dwRet = 0;
	ReadFile(hFile, pData, dwFileSize, &dwRet, NULL);
	CloseHandle(hFile);

	// �ж������ض�λ��
	if (FALSE == IsExistRelocationTable(pData))
	{
		printf("[FALSE] IsExistRelocationTable\n");
		system("pause");
		return 0;
	}
	// ���ڴ�DLL���ص�������
	LPVOID lpBaseAddress = MmRunExe(pData, dwFileSize);
	if (NULL == lpBaseAddress)
	{
		ShowError("MmRunExe");
		return 3;
	}

	system("pause");
	return 0;
}

