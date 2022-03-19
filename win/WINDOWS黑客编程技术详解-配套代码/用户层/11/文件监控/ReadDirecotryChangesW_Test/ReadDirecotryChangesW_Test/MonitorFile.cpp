#include "stdafx.h"
#include "MonitorFile.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
}


// ���ֽ��ַ���ת���ֽ��ַ���
void W2C(wchar_t *pwszSrc, int iSrcLen, char *pszDest, int iDestLen)
{
	::RtlZeroMemory(pszDest, iDestLen);
	// ���ֽ��ַ���ת���ֽ��ַ���
	::WideCharToMultiByte(CP_ACP,
		0,
		pwszSrc,
		(iSrcLen / 2),
		pszDest,
		iDestLen,
		NULL,
		NULL);
}


// Ŀ¼��ض��߳�
UINT MonitorFileThreadProc(LPVOID lpVoid)
{
	char *pszDirectory = (char *)lpVoid;

	// ��Ŀ¼, ��ȡ�ļ����
	HANDLE hDirectory = ::CreateFile(pszDirectory, FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (INVALID_HANDLE_VALUE == hDirectory)
	{
		ShowError("CreateFile");
		return 1;
	}

	char szTemp[MAX_PATH] = { 0 };
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	DWORD dwBufferSize = 2048;
	
	// ����һ���㹻��Ļ����� 
	BYTE *pBuf = new BYTE[dwBufferSize];
	if (NULL == pBuf)
	{
		ShowError("new");
		return 2;
	}
	FILE_NOTIFY_INFORMATION *pFileNotifyInfo = (FILE_NOTIFY_INFORMATION *)pBuf;

	// ��ʼѭ�����ü��
	do
	{
		::RtlZeroMemory(pFileNotifyInfo, dwBufferSize);
		// ���ü��Ŀ¼
		bRet = ::ReadDirectoryChangesW(hDirectory,
			pFileNotifyInfo,
			dwBufferSize,
			TRUE,
			FILE_NOTIFY_CHANGE_FILE_NAME |			// �޸��ļ���
			FILE_NOTIFY_CHANGE_ATTRIBUTES |			// �޸��ļ�����
			FILE_NOTIFY_CHANGE_LAST_WRITE,			// ���һ��д��
			&dwRet,
			NULL,
			NULL);
		if (FALSE == bRet)
		{
			ShowError("ReadDirectoryChangesW");
			break;
		}
		// �����ַ�ת����խ�ַ�
		W2C((wchar_t *)(&pFileNotifyInfo->FileName), pFileNotifyInfo->FileNameLength, szTemp, MAX_PATH);
		// �жϲ������Ͳ���ʾ
		switch (pFileNotifyInfo->Action)
		{
		case FILE_ACTION_ADDED:
		{
			// �����ļ�
			printf("[File Added Action]%s\n", szTemp);
			break;
		}
		default:
		{
			break;
		}
		}


	} while (bRet);
	// �رվ��, �ͷ��ڴ�
	::CloseHandle(hDirectory);
	delete[] pBuf;
	pBuf = NULL;

	return 0;
}


// ����Ŀ¼��ض��߳�
void MonitorFile(char *pszDirectory)
{
	// �����ļ���ض��߳�
	::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MonitorFileThreadProc, pszDirectory, 0, NULL);
}