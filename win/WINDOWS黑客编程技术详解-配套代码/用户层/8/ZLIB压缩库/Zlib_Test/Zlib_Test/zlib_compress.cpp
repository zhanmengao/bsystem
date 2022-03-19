#include "stdafx.h"
#include "zlib_compress.h"


void Zlib_ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


// ����ѹ��
// ���룺��Ҫѹ���ļ���·��
// ���������ѹ�����ѹ���������ݡ�����ѹ�����ѹ���������ݳ���
BOOL Zlib_CompressData(char *pszCompressFileName, BYTE **ppCompressData, DWORD *pdwCompressDataSize)
{
	// ע����ܳ���ѹ������ļ���ѹ��ǰ���ļ��������!!!

	// ���ļ� �� ��ȡ�ļ�����
	HANDLE hFile = ::CreateFile(pszCompressFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Zlib_ShowError("CreateFile");
		return FALSE;
	}

	DWORD dwFileSize = ::GetFileSize(hFile, NULL);       // ��ȡ�ļ���С
	if (MAX_SRC_FILE_SIZE < dwFileSize)
	{
		::CloseHandle(hFile);
		return FALSE;
	}

	// �ж��Ƿ������С��������
	if (MAX_SRC_FILE_SIZE < dwFileSize)
	{
		::CloseHandle(hFile);
		return FALSE;
	}
	DWORD dwDestDataSize = dwFileSize;

	BYTE *pSrcData = new BYTE[dwFileSize];
	if (NULL == pSrcData)
	{
		::CloseHandle(hFile);
		return FALSE;
	}
	BYTE *pDestData = new BYTE[dwDestDataSize];
	if (NULL == pDestData)
	{
		::CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;
	::ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);	 // ��ȡ�ļ�����
	if ((0 >= dwRet) ||
		(dwRet != dwFileSize))
	{
		delete[]pDestData;
		pDestData = NULL;
		delete[]pSrcData;
		pSrcData = NULL;
		::CloseHandle(hFile);
		return FALSE;
	}

	// ѹ������
	/*
	int compress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
	compress������source�������е�����ѹ����dest��������sourceLen��ʾsource�������Ĵ�С�����ֽڼƣ���
	ע�⣺�����ĵڶ�������destLen�Ǵ�ַ���ã������ú���ʱ��destLen��ʾdest��������С����ʼֵ����Ϊ0Ŷ����
	( destLen > (sourceLen + 12) * 100.1% )���������˳���destLen��ʾѹ���󻺳�����ʵ�ʴ�С��
	��ʱ��destLen/sourceLen������ѹ����!!!
	����ֵ��
	-5 : ���������������
	-4 : û���㹻���ڴ棻
	0 : ��ʾ�ɹ���
	*/
	int iRet = 0;
	do
	{
		iRet = compress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0 == iRet)
		{
			// �ɹ�
			break;
		}
		else if (-5 == iRet)
		{
			// ���������������, �� 100KB ��С����
			delete[]pDestData;
			pDestData = NULL;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (NULL == pDestData)
			{
				delete[]pSrcData;
				pSrcData = NULL;
				::CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			// û���㹻���ڴ� �� �������
			delete[]pDestData;
			pDestData = NULL;
			delete[]pSrcData;
			pSrcData = NULL;
			::CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);
	// ��������
	*ppCompressData = pDestData;
	*pdwCompressDataSize = dwDestDataSize;

	// �ͷ�
	//	delete[]pDestData;
	//	pDestData = NULL;
	delete[]pSrcData;
	pSrcData = NULL;
	::CloseHandle(hFile);

	return TRUE;
}


// ���ݽ�ѹ
// ���룺��Ҫ��ѹ���ļ���·��
// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
BOOL Zlib_UncompressData(char *pszUncompressFileName, BYTE **ppUncompressData, DWORD *pdwUncompressDataSize)
{
	// ע����ܳ���ѹ������ļ���ѹ��ǰ���ļ��������!!!

	// ���ļ� �� ��ȡ�ļ�����
	HANDLE hFile = ::CreateFile(pszUncompressFileName, GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Zlib_ShowError("CreateFile");
		return FALSE;
	}

	DWORD dwFileSize = ::GetFileSize(hFile, NULL);       // ��ȡ�ļ���С
	DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;

	BYTE *pSrcData = new BYTE[dwFileSize];
	if (NULL == pSrcData)
	{
		::CloseHandle(hFile);
		return FALSE;
	}
	BYTE *pDestData = new BYTE[dwDestDataSize];
	if (NULL == pDestData)
	{
		::CloseHandle(hFile);
		return FALSE;
	}

	DWORD dwRet = 0;
	::ReadFile(hFile, pSrcData, dwFileSize, &dwRet, NULL);	 // ��ȡ�ļ�����
	if ((0 >= dwRet) ||
		(dwRet != dwFileSize))
	{
		delete[]pDestData;
		pDestData = NULL;
		delete[]pSrcData;
		pSrcData = NULL;
		::CloseHandle(hFile);
		return FALSE;
	}

	// ��ѹ������
	/*
	int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
	compress������source�������е�����ѹ����dest��������sourceLen��ʾsource�������Ĵ�С�����ֽڼƣ���
	ע�⣺�����ĵڶ�������destLen�Ǵ�ַ���ã������ú���ʱ��destLen��ʾdest��������С����ʼֵ����Ϊ0Ŷ����
	( destLen > (sourceLen + 12) * 100.1% )���������˳���destLen��ʾѹ���󻺳�����ʵ�ʴ�С��
	��ʱ��destLen/sourceLen������ѹ����!!!
	����ֵ��
	-5 : ���������������
	-4 : û���㹻���ڴ棻
	0 : ��ʾ�ɹ���
	*/
	int iRet = 0;
	do
	{
		iRet = uncompress(pDestData, &dwDestDataSize, pSrcData, dwFileSize);
		if (0 == iRet)
		{
			// �ɹ�
			break;
		}
		else if (-5 == iRet)
		{
			// ���������������, �� 100KB ��С����
			delete[]pDestData;
			pDestData = NULL;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
			if (NULL == pDestData)
			{
				delete[]pSrcData;
				pSrcData = NULL;
				::CloseHandle(hFile);
				return FALSE;
			}
		}
		else
		{
			// û���㹻���ڴ� �� �������
			delete[]pDestData;
			pDestData = NULL;
			delete[]pSrcData;
			pSrcData = NULL;
			::CloseHandle(hFile);
			return FALSE;
		}
	} while (TRUE);
	// ��������
	*ppUncompressData = pDestData;
	*pdwUncompressDataSize = dwDestDataSize;

	// �ͷ�
	//	delete[]pDestData;
	//	pDestData = NULL;
	delete[]pSrcData;
	pSrcData = NULL;
	::CloseHandle(hFile);

	return TRUE;
}


// ���ݽ�ѹ
// ���룺��Ҫ��ѹ�����������ݡ���Ҫ��ѹ�����������ݳ���
// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
BOOL Zlib_UncompressData(BYTE *pCompressData, DWORD dwCompressDataSize, BYTE **ppUncompressData, DWORD *pdwUncompressDataSize)
{
	// ע����ܳ���ѹ������ļ���ѹ��ǰ���ļ��������!!!

	// ���붯̬�ڴ�
	DWORD dwDestDataSize = MAX_SRC_FILE_SIZE;
	BYTE *pDestData = new BYTE[dwDestDataSize];
	if (NULL == pDestData)
	{
		return FALSE;
	}

	// ��ѹ������
	/*
	int uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);
	compress������source�������е�����ѹ����dest��������sourceLen��ʾsource�������Ĵ�С�����ֽڼƣ���
	ע�⣺�����ĵڶ�������destLen�Ǵ�ַ���ã������ú���ʱ��destLen��ʾdest��������С����ʼֵ����Ϊ0Ŷ����
	( destLen > (sourceLen + 12) * 100.1% )���������˳���destLen��ʾѹ���󻺳�����ʵ�ʴ�С��
	��ʱ��destLen/sourceLen������ѹ����!!!
	����ֵ��
	-5 : ���������������
	-4 : û���㹻���ڴ棻
	0 : ��ʾ�ɹ���
	*/
	int iRet = 0;
	do
	{
		iRet = uncompress(pDestData, &dwDestDataSize, pCompressData, dwCompressDataSize);
		if (0 == iRet)
		{
			// �ɹ�
			break;
		}
		else if (-5 == iRet)
		{
			// ���������������, �� 100KB ��С����
			delete[]pDestData;
			pDestData = NULL;
			dwDestDataSize = dwDestDataSize + (100 * 1024);
			pDestData = new BYTE[dwDestDataSize];
		}
		else
		{
			// û���㹻���ڴ� �� �������
			delete[]pDestData;
			pDestData = NULL;
			return FALSE;
		}
	} while (TRUE);
	// ��������
	*ppUncompressData = pDestData;
	*pdwUncompressDataSize = dwDestDataSize;

	// �ͷ�
	//	delete[]pDestData;
	//	pDestData = NULL;

	return TRUE;
}


void Zlib_GetCurrentFile(char *pszCurrentFile, DWORD dwBufferSize)
{
	char szFileName[MAX_PATH] = { 0 };
	::RtlZeroMemory(szFileName, MAX_PATH);

	::GetModuleFileName(NULL, szFileName, MAX_PATH);
	char *p = ::strrchr(szFileName, '\\');
	p[0] = '\0';

	::lstrcat(szFileName, "\\");
	::lstrcat(szFileName, pszCurrentFile);

	::RtlZeroMemory(pszCurrentFile, dwBufferSize);
	::lstrcpy(pszCurrentFile, szFileName);
}


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize)
{
	char szSaveName[MAX_PATH] = {0};
	::lstrcpy(szSaveName, pszFileName);
	::PathStripPath(szSaveName);
	Zlib_GetCurrentFile(szSaveName, MAX_PATH);

	HANDLE hFile = ::CreateFile(szSaveName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Zlib_ShowError("CreateFile");
		return FALSE;
	}

	DWORD dwRet = 0;
	::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);

	::CloseHandle(hFile);

	return TRUE;
}


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL SaveToOriginalFile(char *pszFileName, BYTE *pData, DWORD dwDataSize)
{
	char szSaveName[MAX_PATH] = { 0 };
	::lstrcpy(szSaveName, pszFileName);
	::PathStripPath(szSaveName);
	// �����ļ�
	HANDLE hFile = ::CreateFile(szSaveName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		Zlib_ShowError("CreateFile");
		return FALSE;
	}
	// д������
	DWORD dwRet = 0;
	::WriteFile(hFile, pData, dwDataSize, &dwRet, NULL);
	// �رվ��
	::CloseHandle(hFile);

	return TRUE;
}