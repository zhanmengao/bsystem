#include "stdafx.h"
#include "WinInet_Ftp.h"


void Ftp_ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = {0};
	::wsprintf(szErr, "%s Error[%d]\n", lpszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK);
#endif
}


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL Ftp_SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize)
{
	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Ftp_ShowError("CreateFile");
		return FALSE;
	}

	DWORD dwRet = 0;
	::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);

	::CloseHandle(hFile);

	return TRUE;
}


// URL�ֽ�
// ���룺URL
// �����szScheme��szHostName��szUserName��szPassword��szUrlPath��szExtraInfo
BOOL Ftp_UrlCrack(char *pszUrl, char *pszScheme, char *pszHostName, char *pszUserName, char *pszPassword, char *pszUrlPath, char *pszExtraInfo, DWORD dwBufferSize)
{
	BOOL bRet = FALSE;
	URL_COMPONENTS uc = { 0 };

	::RtlZeroMemory(&uc, sizeof(uc));
	::RtlZeroMemory(pszScheme, dwBufferSize);
	::RtlZeroMemory(pszHostName, dwBufferSize);
	::RtlZeroMemory(pszUserName, dwBufferSize);
	::RtlZeroMemory(pszPassword, dwBufferSize);
	::RtlZeroMemory(pszUrlPath, dwBufferSize);
	::RtlZeroMemory(pszExtraInfo, dwBufferSize);

	uc.dwStructSize = sizeof(uc);
	uc.dwSchemeLength = dwBufferSize - 1;
	uc.dwHostNameLength = dwBufferSize - 1;
	uc.dwUserNameLength = dwBufferSize - 1;
	uc.dwPasswordLength = dwBufferSize - 1;
	uc.dwUrlPathLength = dwBufferSize - 1;
	uc.dwExtraInfoLength = dwBufferSize - 1;
	uc.lpszScheme = pszScheme;
	uc.lpszHostName = pszHostName;
	uc.lpszUserName = pszUserName;
	uc.lpszPassword = pszPassword;
	uc.lpszUrlPath = pszUrlPath;
	uc.lpszExtraInfo = pszExtraInfo;

	// �ֽ�
	bRet = ::InternetCrackUrl(pszUrl, 0, 0, &uc);
	if (FALSE == bRet)
	{
		Ftp_ShowError("InternetCrackUrl");
		return bRet;
	}

	return bRet;
}


// ��������
// ���룺�������ݵ�URL·��
// ����������������ݡ������������ݳ���
BOOL FTPDownload(char *pszDownloadUrl, BYTE **ppDownloadData, DWORD *pdwDownloadDataSize)
{
	// INTERNET_SCHEME_HTTPS��INTERNET_SCHEME_HTTP��INTERNET_SCHEME_FTP��
	char szScheme[MAX_PATH] = { 0 };
	char szHostName[MAX_PATH] = { 0 };
	char szUserName[MAX_PATH] = { 0 };
	char szPassword[MAX_PATH] = { 0 };
	char szUrlPath[MAX_PATH] = { 0 };
	char szExtraInfo[MAX_PATH] = { 0 };
	::RtlZeroMemory(szScheme, MAX_PATH);
	::RtlZeroMemory(szHostName, MAX_PATH);
	::RtlZeroMemory(szUserName, MAX_PATH);
	::RtlZeroMemory(szPassword, MAX_PATH);
	::RtlZeroMemory(szUrlPath, MAX_PATH);
	::RtlZeroMemory(szExtraInfo, MAX_PATH);
	// �ֽ�URL
	if (FALSE == Ftp_UrlCrack(pszDownloadUrl, szScheme, szHostName, szUserName, szPassword, szUrlPath, szExtraInfo, MAX_PATH))
	{
		return FALSE;
	}
	if (0 < ::lstrlen(szExtraInfo))
	{
		// ע��˴�������
		::lstrcat(szUrlPath, szExtraInfo);
	}

	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hFTPFile = NULL;
	BYTE *pDownloadData = NULL;
	DWORD dwDownloadDataSize = 0;
	DWORD dwBufferSize = 4096;
	BYTE *pBuf = NULL;
	DWORD dwBytesReturn = 0;
	DWORD dwOffset = 0;
	BOOL bRet = FALSE;

	do
	{
		// �����Ự
		hInternet = ::InternetOpen("WinInet Ftp Download V1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (NULL == hInternet)
		{
			Ftp_ShowError("InternetOpen");
			break;
		}
		// ��������
		hConnect = ::InternetConnect(hInternet, szHostName, INTERNET_INVALID_PORT_NUMBER, szUserName, szPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		if (NULL == hConnect)
		{
			Ftp_ShowError("InternetConnect");
			break;
		}
		// ��FTP�ļ�, �ļ������ͱ��ز�������
		hFTPFile = ::FtpOpenFile(hConnect, szUrlPath, GENERIC_READ, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL);
		if (NULL == hFTPFile)
		{
			Ftp_ShowError("FtpOpenFile");
			break;;
		}
		// ��ȡ�ļ���С
		dwDownloadDataSize = ::FtpGetFileSize(hFTPFile, NULL);
		// ���붯̬�ڴ�
		pDownloadData = new BYTE[dwDownloadDataSize];
		if (NULL == pDownloadData)
		{
			break;
		}
		::RtlZeroMemory(pDownloadData, dwDownloadDataSize);
		pBuf = new BYTE[dwBufferSize];
		if (NULL == pBuf)
		{
			break;
		}
		::RtlZeroMemory(pBuf, dwBufferSize);
		// ��������
		do
		{
			bRet = ::InternetReadFile(hFTPFile, pBuf, dwBufferSize, &dwBytesReturn);
			if (FALSE == bRet)
			{
				Ftp_ShowError("InternetReadFile");
				break;
			}
			::RtlCopyMemory((pDownloadData + dwOffset), pBuf, dwBytesReturn);
			dwOffset = dwOffset + dwBytesReturn;

		} while (dwDownloadDataSize > dwOffset);

	} while (FALSE);
	// ��������
	if (FALSE == bRet)
	{
		delete[]pDownloadData;
		pDownloadData = NULL;
		dwDownloadDataSize = 0;
	}
	*ppDownloadData = pDownloadData;
	*pdwDownloadDataSize = dwDownloadDataSize;

	// �ͷ��ڴ沢�رվ��
	if (NULL != pBuf)
	{
		delete []pBuf;
		pBuf = NULL;
	}
	if (NULL != hFTPFile)
	{
		::InternetCloseHandle(hFTPFile);
	}
	if (NULL != hConnect)
	{
		::InternetCloseHandle(hConnect);
	}
	if (NULL != hInternet)
	{
		::InternetCloseHandle(hInternet);
	}

	return bRet;
}


// �����ϴ�
// ���룺�ϴ����ݵ�URL·�����ϴ��������ݡ��ϴ��������ݳ���
BOOL FTPUpload(char *pszUploadUrl, BYTE *pUploadData, DWORD dwUploadDataSize)
{
	// INTERNET_SCHEME_HTTPS��INTERNET_SCHEME_HTTP��INTERNET_SCHEME_FTP��
	char szScheme[MAX_PATH] = { 0 };
	char szHostName[MAX_PATH] = { 0 };
	char szUserName[MAX_PATH] = { 0 };
	char szPassword[MAX_PATH] = { 0 };
	char szUrlPath[MAX_PATH] = { 0 };
	char szExtraInfo[MAX_PATH] = { 0 };
	::RtlZeroMemory(szScheme, MAX_PATH);
	::RtlZeroMemory(szHostName, MAX_PATH);
	::RtlZeroMemory(szUserName, MAX_PATH);
	::RtlZeroMemory(szPassword, MAX_PATH);
	::RtlZeroMemory(szUrlPath, MAX_PATH);
	::RtlZeroMemory(szExtraInfo, MAX_PATH);
	// �ֽ�URL
	if (FALSE == Ftp_UrlCrack(pszUploadUrl, szScheme, szHostName, szUserName, szPassword, szUrlPath, szExtraInfo, MAX_PATH))
	{
		return FALSE;
	}
	if (0 < ::lstrlen(szExtraInfo))
	{
		// ע��˴�������
		::lstrcat(szUrlPath, szExtraInfo);
	}

	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hFTPFile = NULL;
	DWORD dwBytesReturn = 0;
	BOOL bRet = FALSE;

	do
	{
		// �����Ự
		hInternet = ::InternetOpen("WinInet Ftp Upload V1.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (NULL == hInternet)
		{
			Ftp_ShowError("InternetOpen");
			break;
		}
		// ��������
		hConnect = ::InternetConnect(hInternet, szHostName, INTERNET_INVALID_PORT_NUMBER, szUserName, szPassword, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
		if (NULL == hConnect)
		{
			Ftp_ShowError("InternetConnect");
			break;
		}
		// ��FTP�ļ�, �ļ������ͱ��ز�������
		hFTPFile = ::FtpOpenFile(hConnect, szUrlPath, GENERIC_WRITE, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, NULL);
		if (NULL == hFTPFile)
		{
			Ftp_ShowError("FtpOpenFile");
			break;;
		}
		// �ϴ�����
		bRet = ::InternetWriteFile(hFTPFile, pUploadData, dwUploadDataSize, &dwBytesReturn);
		if (FALSE == bRet)
		{
			break;
		}

	} while (FALSE);

	// �ͷ��ڴ沢�رվ��
	if (NULL != hFTPFile)
	{
		::InternetCloseHandle(hFTPFile);
	}
	if (NULL != hConnect)
	{
		::InternetCloseHandle(hConnect);
	}
	if (NULL != hInternet)
	{
		::InternetCloseHandle(hInternet);
	}

	return bRet;
}