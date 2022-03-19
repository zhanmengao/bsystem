#include "stdafx.h"
#include "FileSearch.h"


void SearchFile(char *pszDirectory)
{
	// ����ָ�������ļ�
	DWORD dwBufferSize = 2048;
	char *pszFileName = NULL;
	char *pTempSrc = NULL;
	WIN32_FIND_DATA FileData = {0};
	BOOL bRet = FALSE;
	
	// ���붯̬�ڴ�
	pszFileName = new char[dwBufferSize];
	pTempSrc = new char[dwBufferSize];
	
	// ���������ļ������ַ���, *.*��ʾ���������ļ�����
	::wsprintf(pszFileName, "%s\\*.*", pszDirectory);

	// ������һ���ļ�
	HANDLE hFile = ::FindFirstFile(pszFileName, &FileData);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		do
		{
			// Ҫ���˵� ��ǰĿ¼"." �� ��һ��Ŀ¼"..", ����᲻�Ͻ�����ѭ������
			if ('.' == FileData.cFileName[0])
			{
				continue;  
			}
			// ƴ���ļ�·��	
			::wsprintf(pTempSrc, "%s\\%s", pszDirectory, FileData.cFileName);
			// �ж��Ƿ���Ŀ¼�����ļ�
			if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)        
			{
				// Ŀ¼, ��������µݹ�����ļ�
				SearchFile(pTempSrc);
			}
			else                                                            
			{
				// �ļ�
				printf("%s\n", pTempSrc);
			}

		    // ������һ���ļ�
		} while (::FindNextFile(hFile, &FileData));
	}

	// �ر��ļ����
	::FindClose(hFile);
	// �ͷ��ڴ�
	delete []pTempSrc;
	pTempSrc = NULL;
	delete []pszFileName;
	pszFileName = NULL;
}