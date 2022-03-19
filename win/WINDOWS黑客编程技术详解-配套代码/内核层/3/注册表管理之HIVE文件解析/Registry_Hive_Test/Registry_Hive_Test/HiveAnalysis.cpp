#include "stdafx.h"
#include "HiveAnalysis.h"


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[%d]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


BOOL AnalysisHiveFile(char *pszHiveFileName)
{
	BOOL bRet = FALSE;
	HANDLE hFile = NULL, hFileMap = NULL;
	LPVOID pMemory = NULL;

	do
	{
		// �ڴ�ӳ���ļ�
		hFile = ::CreateFile(pszHiveFileName, GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_ARCHIVE, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			ShowError("CreateFile");
			break;
		}
		hFileMap = ::CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
		if (NULL == hFileMap)
		{
			ShowError("CreateFileMapping");
			break;
		}
		pMemory = MapViewOfFile(hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
		if (NULL == pMemory)
		{
			ShowError("MapViewOfFile");
			break;
		}

		// ����HIVE�ļ�ͷ
		bRet = AnalysisHiveHeader((PUCHAR)pMemory);

	} while (FALSE);

	// �ͷ�
	if (pMemory)
	{
		UnmapViewOfFile(pMemory);
	}
	if (hFileMap)
	{
		::CloseHandle(hFileMap);
	}
	if (hFile)
	{
		::CloseHandle(hFile);
	}

	return bRet;
}


// ����HIVE�ļ�ͷ
BOOL AnalysisHiveHeader(PUCHAR pMemory)
{
	BOOL bRet = FALSE;
	DWORD dwMajor = 0, dwMinor = 0;
	DWORD dwRootCellOffset = 0;
	DWORD dwLength = 0;
	WCHAR *pwszHiveName = NULL;
	PUCHAR pHBIN = NULL;

	// ��ȡHIVE�ļ����汾��
	dwMajor = *(DWORD *)(pMemory + 0x14);
	// ��ȡHIVE�ļ��ΰ汾��
	dwMinor = *(DWORD *)(pMemory + 0x18);
	// ��ȡRootCellOffset
	dwRootCellOffset = *(DWORD *)(pMemory + 0x24);
	// ��ȡHIVE�ļ��ܳ���
	dwLength = *(DWORD *)(pMemory + 0x28);
	// HIVE�ļ�����
	pwszHiveName = (WCHAR *)(pMemory + 0x30);

	// ��ʾ
	printf("-----------------------------------------------------------------------\n");
	printf("dwMajor=%d, dwMinor=%d\n", dwMajor, dwMinor);
	printf("RootCellOffset=0x%X, dwLength=0x%X\n", dwRootCellOffset, dwLength);
	printf("szHiveName=%S\n", pwszHiveName);
	printf("-----------------------------------------------------------------------\n\n");

	// ����hbin
	pHBIN = pMemory + 0x1000;
	// ����NK
	bRet = HiveNK(pHBIN, (pHBIN + dwRootCellOffset));

	return bRet;
}


// ����NK
BOOL HiveNK(PUCHAR pHBIN, PUCHAR pNode)
{
	char *pszNodeName = NULL;
	USHORT usSignature = 0;
	DWORD dwSubNodeCount = 0;
	DWORD dwSubNodeOffset = 0;
	DWORD dwValueCount = 0;
	DWORD dwValueOffset = 0;
	DWORD dwNodeNameLength = 0;
	DWORD dwValueOffsetList = NULL;
	DWORD i = 0;

	// ��ȡ ǩ��
	usSignature = *(USHORT *)(pNode + 0x4);
	if (0x6B6E != usSignature)  // nk
	{
		return FALSE;
	}
	// ��ȡ �Ӽ�����
	dwSubNodeCount = *(DWORD *)(pNode + 0x18);
	// ��ȡ �Ӽ�����
	dwSubNodeOffset = *(DWORD *)(pNode + 0x20);
	// ��ȡ ��ֵ����
	dwValueCount = *(DWORD *)(pNode + 0x28);
	// ��ȡ ��ֵ����
	dwValueOffset = *(DWORD *)(pNode + 0x2C);
	// ��ȡ ��������
	dwNodeNameLength = *(DWORD *)(pNode + 0x4C);
	// ��ȡ ����
	pszNodeName = (char *)(pNode + 0x50);

	// ��ʾ
	for (i = 0; i < dwNodeNameLength; i++)
	{
		printf("%c", pszNodeName[i]);
	}
	printf("\n");

	// ������ֵ
	for (i = 0; i < dwValueCount; i++)
	{
		// ����VK 
		DWORD dwOffset = *(DWORD *)(pHBIN + dwValueOffset + 4 * (1 + i));
		HiveVK(pHBIN, (pHBIN + dwOffset));
	}

	// �����Ӽ�
	for (i = 0; i < dwSubNodeCount; i++)
	{
		// ����LIST
		HiveList(pHBIN, (pHBIN + dwSubNodeOffset));
	}

	return TRUE;
}


// ����VK
BOOL HiveVK(PUCHAR pHBIN, PUCHAR pValue)
{
	char *pszValueName = NULL;
	USHORT usSignature = 0;
	USHORT usValueNameLength = 0;
	DWORD dwValueDataLength = 0;
	DWORD dwValueData = 0;
	DWORD dwValueType = 0;
	DWORD i = 0;

	// ��ȡ ǩ��
	usSignature = *(USHORT *)(pValue + 0x4);
	if (0x6B76 != usSignature) //vk
	{
		return FALSE;
	}
	// ��ȡ ��ֵ���Ƴ���
	usValueNameLength = *(USHORT *)(pValue + 0x6);
	// ��ȡ ��ֵ���ݳ���
	dwValueDataLength = *(DWORD *)(pValue + 0x8);
	// ��ȡ ��ֵ����
	dwValueData = *(DWORD *)(pValue + 0xC);
	// ��ȡ ��ֵ��������
	dwValueType = *(DWORD *)(pValue + 0x10);
	// ��ȡ ��ֵ����
	pszValueName = (char *)(pValue + 0x18);

	// ��ʾ
	// ��ֵ��������
	for (i = 0; i < usValueNameLength; i++)
	{
		printf("%c", pszValueName[i]);
	}
	// ��ֵ��������
	printf(", %d\n", dwValueType);

	/*
	printf(", %d, ", dwValueType);
	// ��ֵ����
	// �ж��Ƿ���פ������
	if (0x80000000 < dwValueDataLength)
	{
		// פ������
		dwValueDataLength = dwValueDataLength - 0x80000000;
		switch (dwValueDataLength)
		{
		case 1:
		{
			dwValueData = dwValueData & 0x000000FF;
			break;
		}
		case 2:
		{
			dwValueData = dwValueData & 0x0000FFFF;
			break;
		}
		case 3:
		{
			dwValueData = dwValueData & 0x00FFFFFF;
			break;
		}
		case 4:
		{
			dwValueData = dwValueData & 0xFFFFFFFF;
			break;
		}
		default:
			break;
		}
		printf("%X\n", dwValueData);
	}
	else
	{
		// ��פ������
		for (i = 0; i < dwValueDataLength; i++)
		{
			printf("0x%X ", *(UCHAR *)(pHBIN + dwValueData + 0x4 + i));
		}
		printf("\n");
	}
	**/
	return TRUE;
}


// ����LIST
BOOL HiveList(PUCHAR pHBIN, PUCHAR pList)
{
	USHORT usSignature = 0;
	USHORT usCount = 0;
	DWORD dwOffset = 0;
	USHORT i = 0;

	// ��ȡ ǩ��
	usSignature = *(USHORT *)(pList + 0x4);
	// ��ȡ ����
	usCount = *(USHORT *)(pList + 0x6);

	// �ж�
	if (0x666C == usSignature ||      // lf      
		0x686C == usSignature)        // lh
	{
		// ��ȡƫ����
		for (i = 0; i < usCount; i++)
		{
			dwOffset = *(DWORD *)(pList + 0x8 + 8 * i);
			HiveNK(pHBIN, (pHBIN + dwOffset));
		}
	}
	else if (0x696C == usSignature || // li
		     0x6972 == usSignature)   // ri
	{
		// ��ȡƫ����
		for (i = 0; i < usCount; i++)
		{
			dwOffset = *(DWORD *)(pList + 0x8 + 4 * i);
			HiveList(pHBIN, (pHBIN + dwOffset));
		}
	}
	else if (0x6264 == usSignature)   // db
	{
		// ��ȡƫ����
		dwOffset = *(DWORD *)(pList + 0x8);
		HiveVK(pHBIN, (pHBIN + dwOffset));
	}

	return TRUE;
}