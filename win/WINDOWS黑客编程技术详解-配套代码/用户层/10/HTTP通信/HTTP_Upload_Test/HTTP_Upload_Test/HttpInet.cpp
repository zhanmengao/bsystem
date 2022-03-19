#include "stdafx.h"
#include "HttpInet.h"


void Http_ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, NULL, MB_OK);
#endif
}


// URL�ֽ�
// ���룺URL
// �����szScheme��szHostName��szUserName��szPassword��szUrlPath��szExtraInfo
BOOL Http_UrlCrack(char *pszUrl, char *pszScheme, char *pszHostName, char *pszUserName, char *pszPassword, char *pszUrlPath, char *pszExtraInfo, DWORD dwBufferSize)
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
		Http_ShowError("InternetCrackUrl");
		return bRet;
	}

	return bRet;
}


// ����Ӧ��Ϣͷ��Ϣ�л�ȡ�������ݳ��ȴ�С
// ���룺��Ӧ��Ϣͷ��Ϣ����
// ������������ݳ��ȴ�С
BOOL Http_GetContentLength(char *pResponseHeader, DWORD *pdwContentLength)
{
	// �� ���ֶ� "Content-Length: "(ע���и��ո�) ��ȡ���ݳ���
	int i = 0;
	char szContentLength[MAX_PATH] = { 0 };
	DWORD dwContentLength = 0;
	char szSubStr[] = "Content-Length: ";
	::RtlZeroMemory(szContentLength, MAX_PATH);

	char *p = ::strstr(pResponseHeader, szSubStr);
	if (NULL == p)
	{
		return FALSE;
	}

	//	printf("p=%s\n", p);
	p = p + ::lstrlen(szSubStr);
	while (('0' <= *p) &&
		('9' >= *p))
	{
		szContentLength[i] = *p;
		p++;
		i++;
	}

	dwContentLength = ::atoi(szContentLength);

	*pdwContentLength = dwContentLength;

	return TRUE;
}


// ��������
// ���룺�������ݵ�URL·��
// ����������������ݡ������������ݳ���
BOOL Http_Download(char *pszDownloadUrl, BYTE **ppDownloadData, DWORD *pdwDownloadDataSize)
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
	if (FALSE == Http_UrlCrack(pszDownloadUrl, szScheme, szHostName, szUserName, szPassword, szUrlPath, szExtraInfo, MAX_PATH))
	{
		return FALSE;
	}

	// ��������
	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	DWORD dwOpenRequestFlags = 0;
	BOOL bRet = FALSE;
	unsigned char *pResponseHeaderIInfo = NULL;
	DWORD dwResponseHeaderIInfoSize = 2048;
	BYTE *pBuf = NULL;
	DWORD dwBufSize = 64 * 1024;
	BYTE *pDownloadData = NULL;
	DWORD dwDownloadDataSize = 0;
	DWORD dwRet = 0;
	DWORD dwOffset = 0;

	do
	{
		// �����Ự
		hInternet = ::InternetOpen("WinInetGet/0.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (NULL == hInternet)
		{
			Http_ShowError("InternetOpen");
			break;
		}
		// ��������
		hConnect = ::InternetConnect(hInternet, szHostName, INTERNET_DEFAULT_HTTP_PORT, szUserName, szPassword, INTERNET_SERVICE_HTTP, 0, 0);
		if (NULL == hConnect)
		{
			Http_ShowError("InternetConnect");
			break;
		}
		// �򿪲�����HTTP����
		dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
			INTERNET_FLAG_KEEP_CONNECTION |
			INTERNET_FLAG_NO_AUTH |
			INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_NO_UI |
			INTERNET_FLAG_RELOAD;
		if (0 < ::lstrlen(szExtraInfo))
		{
			// ע��˴�������
			::lstrcat(szUrlPath, szExtraInfo);
		}
		hRequest = ::HttpOpenRequest(hConnect, "GET", szUrlPath, NULL, NULL, NULL, dwOpenRequestFlags, 0);
		if (NULL == hRequest)
		{
			Http_ShowError("HttpOpenRequest");
			break;
		}
		// ��������
		bRet = ::HttpSendRequest(hRequest, NULL, 0, NULL, 0);
		if (FALSE == bRet)
		{
			Http_ShowError("HttpSendRequest");
			break;
		}
		// ������Ӧ�ı�����Ϣͷ(Get Response Header)
		pResponseHeaderIInfo = new unsigned char[dwResponseHeaderIInfoSize];
		if (NULL == pResponseHeaderIInfo)
		{
			break;
		}
		::RtlZeroMemory(pResponseHeaderIInfo, dwResponseHeaderIInfoSize);
		bRet = ::HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pResponseHeaderIInfo, &dwResponseHeaderIInfoSize, NULL);
		if (FALSE == bRet)
		{
			Http_ShowError("HttpQueryInfo");
			break;
		}
#ifdef _DEBUG
		printf("[HTTP_Download_ResponseHeaderIInfo]\n\n%s\n\n", pResponseHeaderIInfo);
#endif
		// �� ���ֶ� "Content-Length: "(ע���и��ո�) ��ȡ���ݳ���
		bRet = Http_GetContentLength((char *)pResponseHeaderIInfo, &dwDownloadDataSize);
		if (FALSE == bRet)
		{
			break;
		}
		// ���ձ�����������(Get Response Body)
		pBuf = new BYTE[dwBufSize];
		if (NULL == pBuf)
		{
			break;
		}
		pDownloadData = new BYTE[dwDownloadDataSize];
		if (NULL == pDownloadData)
		{
			break;
		}
		::RtlZeroMemory(pDownloadData, dwDownloadDataSize);
		do
		{
			::RtlZeroMemory(pBuf, dwBufSize);
			bRet = ::InternetReadFile(hRequest, pBuf, dwBufSize, &dwRet);
			if (FALSE == bRet)
			{
				Http_ShowError("InternetReadFile");
				break;
			}

			::RtlCopyMemory((pDownloadData + dwOffset), pBuf, dwRet);
			dwOffset = dwOffset + dwRet;

		} while (dwDownloadDataSize > dwOffset);

		// ��������
		*ppDownloadData = pDownloadData;
		*pdwDownloadDataSize = dwDownloadDataSize;

	} while (FALSE);

	// �ر� �ͷ�
	if (NULL != pBuf)
	{
		delete[]pBuf;
		pBuf = NULL;
	}
	if (NULL != pResponseHeaderIInfo)
	{
		delete[]pResponseHeaderIInfo;
		pResponseHeaderIInfo = NULL;
	}
	if (NULL != hRequest)
	{
		::InternetCloseHandle(hRequest);
		hRequest = NULL;
	}
	if (NULL != hConnect)
	{
		::InternetCloseHandle(hConnect);
		hConnect = NULL;
	}
	if (NULL != hInternet)
	{
		::InternetCloseHandle(hInternet);
		hInternet = NULL;
	}

	return bRet;
}


// �����ϴ�
// ���룺�ϴ����ݵ�URL·�����ϴ��������ݡ��ϴ��������ݳ���
BOOL Http_Upload(char *pszUploadUrl, BYTE *pUploadData, DWORD dwUploadDataSize)
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
	if (FALSE == Http_UrlCrack(pszUploadUrl, szScheme, szHostName, szUserName, szPassword, szUrlPath, szExtraInfo, MAX_PATH))
	{
		return FALSE;
	}

	// �����ϴ�
	HINTERNET hInternet = NULL;
	HINTERNET hConnect = NULL;
	HINTERNET hRequest = NULL;
	DWORD dwOpenRequestFlags = 0;
	BOOL bRet = FALSE;
	DWORD dwRet = 0;
	unsigned char *pResponseHeaderIInfo = NULL;
	DWORD dwResponseHeaderIInfoSize = 2048;
	BYTE *pBuf = NULL;
	DWORD dwBufSize = 64 * 1024;
	BYTE *pResponseBodyData = NULL;
	DWORD dwResponseBodyDataSize = 0;
	DWORD dwOffset = 0;
	DWORD dwPostDataSize = dwUploadDataSize;
	INTERNET_BUFFERS internetBuffers = { 0 };

	do
	{
		// �����Ự
		hInternet = ::InternetOpen("WinInetPost/0.1", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (NULL == hInternet)
		{
			Http_ShowError("InternetOpen");
			break;
		}
		// ��������
		hConnect = ::InternetConnect(hInternet, szHostName, INTERNET_DEFAULT_HTTP_PORT, szUserName, szPassword, INTERNET_SERVICE_HTTP, 0, 0);
		if (NULL == hConnect)
		{
			Http_ShowError("InternetConnect");
			break;
		}
		// �򿪲�����HTTP����
		dwOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
			INTERNET_FLAG_KEEP_CONNECTION |
			INTERNET_FLAG_NO_AUTH |
			INTERNET_FLAG_NO_COOKIES |
			INTERNET_FLAG_NO_UI |
			INTERNET_FLAG_RELOAD;
		if (0 < ::lstrlen(szExtraInfo))
		{
			// ע��˴�������
			::lstrcat(szUrlPath, szExtraInfo);
		}
		hRequest = ::HttpOpenRequest(hConnect, "POST", szUrlPath, NULL, NULL, NULL, dwOpenRequestFlags, 0);
		if (NULL == hRequest)
		{
			Http_ShowError("HttpOpenRequest");
			break;
		}
		// ��������, ���߷������������ݵ��ܴ�С
		::RtlZeroMemory(&internetBuffers, sizeof(internetBuffers));
		internetBuffers.dwStructSize = sizeof(internetBuffers);
		internetBuffers.dwBufferTotal = dwPostDataSize;
		bRet = ::HttpSendRequestEx(hRequest, &internetBuffers, NULL, 0, 0);
		if (FALSE == bRet)
		{
			break;
		}
		// ��������
		bRet = ::InternetWriteFile(hRequest, pUploadData, dwUploadDataSize, &dwRet);
		if (FALSE == bRet)
		{
			break;
		}
		// �������, ��������
		bRet = ::HttpEndRequest(hRequest, NULL, 0, 0);
		if (FALSE == bRet)
		{
			break;
		}
		// ������Ӧ�ı�����Ϣͷ(Get Response Header)
		pResponseHeaderIInfo = new unsigned char[dwResponseHeaderIInfoSize];
		if (NULL == pResponseHeaderIInfo)
		{
			break;
		}
		::RtlZeroMemory(pResponseHeaderIInfo, dwResponseHeaderIInfoSize);
		bRet = ::HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, pResponseHeaderIInfo, &dwResponseHeaderIInfoSize, NULL);
		if (FALSE == bRet)
		{
			Http_ShowError("HttpQueryInfo");
			break;
		}
#ifdef _DEBUG
		printf("[HTTP_Upload_ResponseHeaderIInfo]\n\n%s\n\n", pResponseHeaderIInfo);
#endif
		// �� ���ֶ� "Content-Length: "(ע���и��ո�) ��ȡ���ݳ���
		bRet = Http_GetContentLength((char *)pResponseHeaderIInfo, &dwResponseBodyDataSize);
		if (FALSE == bRet)
		{
			break;
		}
		// ���ձ�����������(Get Response Body)
		pBuf = new BYTE[dwBufSize];
		if (NULL == pBuf)
		{
			break;
		}
		pResponseBodyData = new BYTE[dwResponseBodyDataSize];
		if (NULL == pResponseBodyData)
		{
			break;
		}
		::RtlZeroMemory(pResponseBodyData, dwResponseBodyDataSize);
		do
		{
			::RtlZeroMemory(pBuf, dwBufSize);
			bRet = ::InternetReadFile(hRequest, pBuf, dwBufSize, &dwRet);
			if (FALSE == bRet)
			{
				Http_ShowError("InternetReadFile");
				break;
			}

			::RtlCopyMemory((pResponseBodyData + dwOffset), pBuf, dwRet);
			dwOffset = dwOffset + dwRet;

		} while (dwResponseBodyDataSize > dwOffset);

	} while (FALSE);

	// �ر� �ͷ�
	if (NULL != pResponseBodyData)
	{
		delete[]pResponseBodyData;
		pResponseBodyData = NULL;
	}
	if (NULL != pBuf)
	{
		delete[]pBuf;
		pBuf = NULL;
	}
	if (NULL != pResponseHeaderIInfo)
	{
		delete[]pResponseHeaderIInfo;
		pResponseHeaderIInfo = NULL;
	}
	
	if (NULL != hRequest)
	{
		::InternetCloseHandle(hRequest);
		hRequest = NULL;
	}
	if (NULL != hConnect)
	{
		::InternetCloseHandle(hConnect);
		hConnect = NULL;
	}
	if (NULL != hInternet)
	{
		::InternetCloseHandle(hInternet);
		hInternet = NULL;
	}

	return bRet;
}


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL Http_SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize)
{
	HANDLE hFile = ::CreateFile(pszFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Http_ShowError("CreateFile");
		return FALSE;
	}

	DWORD dwRet = 0;
	::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);

	::CloseHandle(hFile);

	return TRUE;
}



