// RunDllInMem_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MmLoadDll.h"


int _tmain(int argc, _TCHAR* argv[])
{
	char szFileName[MAX_PATH] = "TestDll.dll";

	// ��DLL�ļ�����ȡDLL�ļ���С
	HANDLE hFile = ::CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ShowError("CreateFile");
		return 1;
	}
	DWORD dwFileSize = ::GetFileSize(hFile, NULL);
	// ���붯̬�ڴ沢��ȡDLL���ڴ���
	BYTE *lpData = new BYTE[dwFileSize];
	if (NULL == lpData)
	{
		ShowError("new");
		return 2;
	}
	DWORD dwRet = 0;
	::ReadFile(hFile, lpData, dwFileSize, &dwRet, NULL);

	// ���ڴ�DLL���ص�������
	LPVOID lpBaseAddress = MmLoadLibrary(lpData, dwFileSize);
	if (NULL == lpBaseAddress)
	{
		ShowError("MmLoadLibrary");
		return 3;
	}
	printf("DLL���سɹ�\n");

	// ��ȡDLL��������������
	typedef BOOL(*typedef_ShowMessage)(char *lpszText, char *lpszCaption);
	typedef_ShowMessage ShowMessage = (typedef_ShowMessage)MmGetProcAddress(lpBaseAddress, "ShowMessage");
	if (NULL == ShowMessage)
	{
		ShowError("MmGetProcAddress");
		return 4;
	}
	ShowMessage("I am Demon��Gan\n", "Who Are You");

	// �ͷŴ��ڴ���ص�DLL
	BOOL bRet = MmFreeLibrary(lpBaseAddress);
	if (FALSE == bRet)
	{
		ShowError("MmFreeLirbary");
	}

	// �ͷ�
	delete[] lpData;
	lpData = NULL;
	::CloseHandle(hFile);

	system("pause");
	return 0;
}

