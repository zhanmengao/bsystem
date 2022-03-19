#include "stdafx.h"
#include "ResourceFree.h"


void FreeRes_ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


BOOL FreeMyResource(UINT uiResouceName, char *lpszResourceType, char *lpszSaveFileName)
{
	// ��ȡָ��ģ�����ָ����Դ
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(uiResouceName), lpszResourceType);
	if (NULL == hRsrc)
	{
		FreeRes_ShowError("FindResource");
		return FALSE;
	}
	// ��ȡ��Դ�Ĵ�С
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		FreeRes_ShowError("SizeofResource");
		return FALSE;
	}
	// ����Դ���ص��ڴ���
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		FreeRes_ShowError("LoadResource");
		return FALSE;
	}
	// ������Դ
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}

	// ������ԴΪ�ļ�
	FILE *fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (NULL == fp)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);

	return TRUE;
}


BOOL GetResourceInfo(UINT uiResouceName, char *lpszResourceType, PVOID *ppVoid, DWORD *pdwResSize)
{
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(uiResouceName), lpszResourceType);
	if (NULL == hRsrc)
	{
		FreeRes_ShowError("FindResource");
		return FALSE;
	}
	DWORD dwSize = ::SizeofResource(NULL, hRsrc);
	if (0 >= dwSize)
	{
		FreeRes_ShowError("SizeofResource");
		return FALSE;
	}
	HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
	if (NULL == hGlobal)
	{
		FreeRes_ShowError("LoadResource");
		return FALSE;
	}
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}

	// ����
	*ppVoid = lpVoid;
	*pdwResSize = dwSize;

	return TRUE;
}


BOOL ChangeIcon(char *pszChangedIconExeFileName, char *pszSrcIconExeFileName)
{
	// �����������򣬲���ȡ����ģ����
	HMODULE hEXE = ::LoadLibrary(pszSrcIconExeFileName);
	if (NULL == hEXE)
	{
		FreeRes_ShowError("LoadLibrary");
		return FALSE;
	}
	// ��ȡ����EXE����ͼ����Դ����
	HRSRC hRsrc = ::FindResource(hEXE, (LPCSTR)1, RT_ICON);
	if (NULL == hRsrc)
	{
		FreeRes_ShowError("FindResource");
		return FALSE;
	}
	// ��ȡ��Դ��С
	DWORD dwSize = ::SizeofResource(hEXE, hRsrc);
	if (0 >= dwSize)
	{
		FreeRes_ShowError("SizeofResource");
		return FALSE;
	}
	// ������Դ�������ڴ�
	HGLOBAL hGlobal = ::LoadResource(hEXE, hRsrc);
	if (NULL == hGlobal)
	{
		FreeRes_ShowError("LoadResource");
		return FALSE;
	}
	// ������Դ�ڴ�
	LPVOID lpVoid = ::LockResource(hGlobal);
	if (NULL == lpVoid)
	{
		FreeRes_ShowError("LockResource");
		return FALSE;
	}

	// ��ʼ�޸�ͼ��
	HANDLE hUpdate = ::BeginUpdateResource(pszChangedIconExeFileName, FALSE);
	if (NULL == hUpdate)
	{
		FreeRes_ShowError("BeginUpdateResource");
		return FALSE;
	}
	// �����ԴID����, ���滻��Դ; ���򴴽���Դ
	// �����ICON����С����ԴID��Ϊ����ͼ��, ���Դ�1��ʼ, 1��С
	BOOL bRet = ::UpdateResource(hUpdate, RT_ICON, (LPCSTR)1, LANG_NEUTRAL, lpVoid, dwSize);
	if (FALSE == bRet)
	{
		FreeRes_ShowError("UpdateResource");
		return FALSE;
	}

	::EndUpdateResource(hUpdate, FALSE);
	// �ͷ�ģ��
	::FreeLibrary(hEXE);

	return TRUE;
}


BOOL ChangeExeRes(char *pszSrcFileName, char *pszInstallFileName, UINT uiDestResId, char *pszDestResType)
{
	BYTE *pData = NULL;
	DWORD dwDataSize = 0;
	// ��Ŀ���ļ���ȡ����
	HANDLE hFile = ::CreateFile(pszInstallFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		FreeRes_ShowError("CreateFile");
		return FALSE;
	}
	// ��ȡ�ļ���С
	dwDataSize = ::GetFileSize(hFile, NULL);
	// ���붯̬�ڴ�
	pData = new BYTE[dwDataSize];
	if (NULL == pData)
	{
		FreeRes_ShowError("new");
		return FALSE;
	}
	// ��ȡ����
	DWORD dwRet = 0;
	if (FALSE == ::ReadFile(hFile, pData, dwDataSize, &dwRet, NULL))
	{
		FreeRes_ShowError("ReadFile");
		return FALSE;
	}
	// �ر��ļ����
	::CloseHandle(hFile);

	// ������Դ
	HANDLE hUpdate = ::BeginUpdateResource(pszSrcFileName, FALSE);
	if (NULL == hUpdate)
	{
		FreeRes_ShowError("BeginUpdateResource");
		return FALSE;
	}

	// �����ԴID����, ���滻��Դ; ���򴴽���Դ
	BOOL bRet = ::UpdateResource(hUpdate, pszDestResType, (LPCSTR)uiDestResId, LANG_NEUTRAL, pData, dwDataSize);
	if (FALSE == bRet)
	{
		FreeRes_ShowError("UpdateResource");
		return FALSE;
	}

	::EndUpdateResource(hUpdate, FALSE);

	// �ͷ��ڴ�
	delete[]pData;
	pData = NULL;

	return TRUE;
}
