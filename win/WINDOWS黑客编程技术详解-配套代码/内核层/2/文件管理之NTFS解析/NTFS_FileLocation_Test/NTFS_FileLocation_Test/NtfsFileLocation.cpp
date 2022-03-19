#include "stdafx.h"
#include "NtfsFileLocation.h"


/*
��λ�ļ�·����F:\test3\iujklm684\test1.txt

1. ��ȡ0x0B��ʼ��2���ֽڣ���ʾÿ�������Ĵ�С:0x200
2. ��ȡ0x0D��ʼ��1���ֽڣ���ʾÿ�����ɼ����������:0x8
3. ��ȡ0x30��ʼ��8���ֽڣ���ʾ$MFT����ʼ�غ�:0xc0000

���Ը���������Ϣ����ó���$MFTԪ�ļ���ƫ���ǣ�
0xc0000 * 0x8 * 0x200 = 0xc0000000

4. ÿ���ļ�Ŀ¼�Ĵ�С�涨��1KB(0x400)����1024�ֽڣ�����
��Ŀ¼��$MFTԪ�ļ�ƫ��5���ļ�����5���ļ�Ŀ¼

���Ը���������Ϣ����ó�����Ŀ¼��ƫ���ǣ�
0xc0000000 + 0x400 * 5 = 0xc0001400

5. �����ļ�Ŀ¼����Ⱥʹ洢���ݵĴ�С��ͬ����ȡ��λʹ�õ�
����Ҳ��ͬ��Ҫע��0x80��0x90��0xA0���������ԣ�
6. ��Ԫ�ļ���Ŀ¼ͷ��ƫ��0x14��ʼ�������ֽڣ��Ǹ�Ŀ¼��
һ�����Ե�ƫ�ƣ�
7. �������У�ǰ4���ֽڣ���ʾ���ԣ�ƫ��0x4��ʼ��4���ֽڣ��Ǹ����ԵĴ�С

�������ϣ����ǿ��Ա��������ļ�Ŀ¼�еĸ������ԣ�
�Դ������ݲ�ͬ������������ͬ�Ĵ���

8. ���Զ�ȡ�����ȼ��ǣ�0xA0 > 0x90 > 0x80��

0xA0��ȡ���8�ֽڣ�
Data Run List:11 01 2c 00 00 00 00 00
(PS:Data Run�����ж�������Ǽ�ס֮���Data RunҪ����֮ǰ��Data�Ĵش�Сƫ�ƣ�����)
�����Data Run List��ʾ���ݵ��߼��غ�����1�ֽڱ�ʾ��2c
���ݵĳ�������1�ֽڱ�ʾ��01
������߼��غ�0x2c��ƫ�ƣ�
0x2c * 0x8 * 0x200 = 0x2c000
����INDX�����У�����UNICODE�ַ������ҵ���������5�У���ȥ0x50�õ��ļ��ţ�
�����еĵ�ַ�ǣ�0x2c7c8,������5�к�0x2c7c8 - 0x50 = 0x2c778
��ȡ0x2c778��ʼ��6���ֽڣ����ɻ���ļ��ţ��ļ��Ŷ�����$MFTΪ��ʼ���ļ��ţ�
ÿ���ļ���С����1KB(0x400)
�ļ����ǣ�2e 00 00 00 00 00
��Ӧ���ļ���Ŀ¼(test3Ŀ¼)��ƫ���ǣ�
0xc0000000 + 0x2e * 0x400 = 0xc000b800
�����µ�Ŀ¼���ˣ��ظ�����8�Ĳ�����

��ζ�ȡ0x90�����ԣ���ɨ��һ��0x90�еĳ�פ�����Ƿ�������Ҫ������
�ļ���Ŀ¼��ʹ��UNICODE���������ɣ����У�����������5�У���ȡ�ļ��ż��ɣ�
��û�У�����Ҫ��ȡ���8�ֽڵ�Data Run��������������ȥUNICODE�������ظ�������

ɨ����һ�£����֡�iujklm684��UNICODE��0x90�������г����ˣ�����������5�У�
��ȡ�ļ���Ϊ��2f 00 00 00 00 00
�����ļ���ƫ�ƣ�
0xc0000000 + 0x2f * 0x400 = 0xc000bc00
�������µ�Ŀ¼�£������ض�����8�Ĳ�����

��0x90�������У���������test1.txt��UNICODE��������5�У�
��ȡǰ6�ֽڣ���ȡ�ļ����ǣ�35 00 00 00 00 00
�����ļ���ƫ�ƣ�
0xc0000000 + 0x35 * 0x400 = 0xc000d400
�������ļ����ˣ����Ը���0x30���������ж��Ƿ���Ŀ¼�����ļ�������

9. �ļ������ݴ洢��0x80�������У�����С�Ļ�����Ϊ��פ����ֱ����0x80������
��ȡ�����ݶ�Ļ�����Ϊ�ǳ�פ���ݴ洢��Data Run�������
*/


// �򿪴���
BOOL OpenDisk(HANDLE &hFile, char *lpszFilePath)
{
	// ��������豸�ַ���
	char szDrive[7] = { '\\', '\\', '.', '\\', 'C', ':', '\0' };
	szDrive[4] = lpszFilePath[0];
	// �򿪴���
	hFile = ::CreateFile(szDrive, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		char szErr[MAX_PATH] = { 0 };
		::wsprintf(szErr, "Create File Error!\nError Code Is:%d\n", ::GetLastError());
		printf("%s", szErr);
		system("pause");
		return FALSE;
	}

	return TRUE;
}


// ��DBR�л�ȡ���ݣ�ÿ�������ֽ�����ÿ���ص���������ԭ�ļ�$MFT����ʼ�غ�
BOOL GetDataFromDBR(HANDLE hFile, WORD &wSizeOfSector, BYTE &bSizeOfCluster, LARGE_INTEGER &liClusterNumberOfMFT)
{
	// ��ȡ������С(2)���ش�С(1)��$MFT��ʼ�غ�(8)
	BYTE bBuffer[512] = { 0 };
	DWORD dwRead = 0;
	// ע�⣺���ݶ�ȡ�Ĵ�С��С��λ������!!!
	::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	::ReadFile(hFile, bBuffer, 512, &dwRead, NULL);
	wSizeOfSector = MAKEWORD(bBuffer[0x0B], bBuffer[0x0C]);
	bSizeOfCluster = bBuffer[0x0D];
	liClusterNumberOfMFT.LowPart = MAKELONG(MAKEWORD(bBuffer[0x30], bBuffer[0x31]), MAKEWORD(bBuffer[0x32], bBuffer[0x33]));
	liClusterNumberOfMFT.HighPart = MAKELONG(MAKEWORD(bBuffer[0x34], bBuffer[0x35]), MAKEWORD(bBuffer[0x36], bBuffer[0x37]));

	return TRUE;
}


// ��λ�ļ�
BOOL LocationFile(HANDLE hFile, char *lpszFilePath, WORD wSizeOfSector, BYTE bSizeOfCluster, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset)
{
	BYTE bBuffer[1024] = { 0 };
	DWORD dwRead = 0;
	// �ָ��ļ�·��
	char szNewFile[MAX_PATH] = { 0 };
	::lstrcpy(szNewFile, (lpszFilePath + 3));
	char szDelim[] = "\\";
	char *lpResult = strtok(szNewFile, szDelim);
	BYTE bUnicode[MAX_PATH] = { 0 };
	while (NULL != lpResult)
	{
		BOOL bFlag = FALSE;
		DWORD dwNameOffset = 0;
		// ���ָ��Ŀ¼ת����2�ֽڱ�ʾ��Unicode����
		DWORD dwLen = ::lstrlen(lpResult);
		::RtlZeroMemory(bUnicode, MAX_PATH);
		for (DWORD i = 0, j = 0; i < dwLen; i++)
		{
			bUnicode[j++] = lpResult[i];
			bUnicode[j++] = 0;
		}

		// ��ȡĿ¼�����ݣ���СΪ1KB
		::SetFilePointer(hFile, liRootOffset.LowPart, &liRootOffset.HighPart, FILE_BEGIN);
		::ReadFile(hFile, bBuffer, 1024, &dwRead, NULL);
		// ��ȡ��һ�����Ե�ƫ��
		WORD wAttributeOffset MAKEWORD(bBuffer[0x14], bBuffer[0x15]);
		// �����ļ�Ŀ¼������
		DWORD dwAttribute = 0;
		DWORD dwSizeOfAttribute = 0;
		while (TRUE)
		{
			if (bFlag)
			{
				break;
			}
			// ��ȡ��ǰ����
			dwAttribute = MAKELONG(MAKEWORD(bBuffer[wAttributeOffset], bBuffer[wAttributeOffset + 1]),
				MAKEWORD(bBuffer[wAttributeOffset + 2], bBuffer[wAttributeOffset + 3]));

			// �ж�����
			if (0x90 == dwAttribute)
			{
				bFlag = HandleAttribute_90(bBuffer, wAttributeOffset, bUnicode, dwLen, liMFTOffset, liRootOffset);
			}
			else if (0xA0 == dwAttribute)
			{
				bFlag = HandleAttribute_A0(hFile, bBuffer, wSizeOfSector, bSizeOfCluster, wAttributeOffset, bUnicode, dwLen, liMFTOffset, liRootOffset);
			}
			else if (0xFFFFFFFF == dwAttribute)
			{
				bFlag = TRUE;
				break;
			}
			// ��ȡ��ǰ���ԵĴ�С
			dwSizeOfAttribute = MAKELONG(MAKEWORD(bBuffer[wAttributeOffset + 4], bBuffer[wAttributeOffset + 5]),
				MAKEWORD(bBuffer[wAttributeOffset + 6], bBuffer[wAttributeOffset + 7]));
			// ������һ���Ե�ƫ��
			wAttributeOffset = wAttributeOffset + dwSizeOfAttribute;
		}

		// �����ָ�Ŀ¼
		lpResult = strtok(NULL, szDelim);
	}

	return TRUE;
}


// 0x90���ԵĴ���
BOOL HandleAttribute_90(BYTE *lpBuffer, WORD wAttributeOffset, BYTE *lpUnicode, DWORD dwLen, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset)
{
	// �ȱ����ж�0x90�������Ƿ��д�Ŀ¼���ļ�(UNICODE)
	// ��ȡ��ǰ���ԵĴ�С
	DWORD dwSizeOfAttribute = MAKELONG(MAKEWORD(lpBuffer[wAttributeOffset + 4], lpBuffer[wAttributeOffset + 5]),
		MAKEWORD(lpBuffer[wAttributeOffset + 6], lpBuffer[wAttributeOffset + 7]));
	for (DWORD i = 0; i < dwSizeOfAttribute; i++)
	{
		if (CompareMemory(lpUnicode, (lpBuffer + wAttributeOffset + i), 2 * dwLen))
		{
			DWORD dwNameOffset = wAttributeOffset + i;
			// �����ļ���
			dwNameOffset = dwNameOffset / 8;
			dwNameOffset = 8 * dwNameOffset;
			dwNameOffset = dwNameOffset - 0x50;
			// ��ȡ�ļ���(6)
			LARGE_INTEGER liNumberOfFile;
			liNumberOfFile.LowPart = MAKELONG(MAKEWORD(lpBuffer[dwNameOffset], lpBuffer[dwNameOffset + 1]),
				MAKEWORD(lpBuffer[dwNameOffset + 2], lpBuffer[dwNameOffset + 3]));
			liNumberOfFile.HighPart = MAKELONG(MAKEWORD(lpBuffer[dwNameOffset + 4], lpBuffer[dwNameOffset + 5]),
				0);

			// �����ļ��ŵ�ƫ��,�ļ��������$MFTΪƫ��˵��
			liRootOffset = liNumberOfFile;
			liRootOffset.QuadPart = liMFTOffset.QuadPart + liRootOffset.QuadPart * 0x400;

			return TRUE;
		}
	}
	// ��ȡData Run List��ȥ��������INDX����UNICODE����ȡ�ļ���

	return FALSE;
}


// 0xA0���ԵĴ���
BOOL HandleAttribute_A0(HANDLE hFile, BYTE *lpBuffer, WORD wSizeOfSector, BYTE bSizeOfCluster, WORD wAttributeOffset, BYTE *lpUnicode, DWORD dwLen, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset)
{
	// ��ȡData Run List��ȥ��������INDX����UNICODE����ȡ�ļ���
	DWORD dwCount = 0;
	DWORD dwClusterOffet = 0;
	// ��ȡ�����ŵ�ƫ��
	WORD wIndxOffset = MAKEWORD(lpBuffer[wAttributeOffset + 0x20], lpBuffer[wAttributeOffset + 0x21]);
	// ��ȡData Run List
	while (TRUE)
	{
		BYTE bTemp = lpBuffer[wAttributeOffset + wIndxOffset + dwCount];
		// ��ȡData Run List,�ֽⲢ����Data Run�е���Ϣ
		BYTE bHi = bTemp >> 4;
		BYTE bLo = bTemp & 0x0F;
		if (0x0F == bHi || 0x0F == bLo || 0 == bHi || 0 == bLo)
		{
			break;
		}
		LARGE_INTEGER liDataRunSize, liDataRunOffset;
		liDataRunSize.QuadPart = 0;
		liDataRunOffset.QuadPart = 0;
		for (DWORD i = bLo; i > 0; i--)
		{
			liDataRunSize.QuadPart = liDataRunSize.QuadPart << 8;
			liDataRunSize.QuadPart = liDataRunSize.QuadPart | lpBuffer[wAttributeOffset + wIndxOffset + dwCount + i];
		}
		for (DWORD i = bHi; i > 0; i--)
		{
			liDataRunOffset.QuadPart = liDataRunOffset.QuadPart << 8;
			liDataRunOffset.QuadPart = liDataRunOffset.QuadPart | lpBuffer[wAttributeOffset + wIndxOffset + dwCount + bLo + i];
		}
		// ע�������һ��Data Run���߼��غ�
		liDataRunOffset.QuadPart = liDataRunOffset.QuadPart + dwClusterOffet;
		dwClusterOffet = dwClusterOffet + liDataRunOffset.LowPart;

		// ȥ��������INDX����UNICODE����ȡ�ļ���
		LARGE_INTEGER liIndxOffset, liIndxSize;
		liIndxOffset.QuadPart = liDataRunOffset.QuadPart * bSizeOfCluster * wSizeOfSector;
		liIndxSize.QuadPart = liDataRunSize.QuadPart * bSizeOfCluster * wSizeOfSector;
		// ��ȡ���������ݣ���СΪ1KB
		BYTE *lpBuf = new BYTE[liIndxSize.QuadPart];
		DWORD dwRead = 0;
		::SetFilePointer(hFile, liIndxOffset.LowPart, &liIndxOffset.HighPart, FILE_BEGIN);
		::ReadFile(hFile, lpBuf, liIndxSize.LowPart, &dwRead, NULL);
		// ����Unicode����
		for (DWORD i = 0; i < liIndxSize.LowPart; i++)
		{
			if (CompareMemory(lpUnicode, (lpBuf + i), 2 * dwLen))
			{
				DWORD dwNameOffset = i;
				// �����ļ���
				dwNameOffset = dwNameOffset / 8;
				dwNameOffset = 8 * dwNameOffset;
				dwNameOffset = dwNameOffset - 0x50;
				// ��ȡ�ļ���(6)
				LARGE_INTEGER liNumberOfFile;
				liNumberOfFile.LowPart = MAKELONG(MAKEWORD(lpBuf[dwNameOffset], lpBuf[dwNameOffset + 1]),
					MAKEWORD(lpBuf[dwNameOffset + 2], lpBuf[dwNameOffset + 3]));
				liNumberOfFile.HighPart = MAKELONG(MAKEWORD(lpBuf[dwNameOffset + 4], lpBuf[dwNameOffset + 5]),
					0);

				// �����ļ��ŵ�ƫ��,�ļ��������$MFTΪƫ��˵��
				liRootOffset = liNumberOfFile;
				liRootOffset.QuadPart = liMFTOffset.QuadPart + liRootOffset.QuadPart * 0x400;

				return TRUE;
			}
		}
		delete[]lpBuf;
		lpBuf = NULL;
		// ������һ��Data Run Listƫ��
		dwCount = dwCount + bLo + bHi + 1;
	}

	return FALSE;
}


// �ڴ�ƥ��
BOOL CompareMemory(BYTE *lpSrc, BYTE *lpDst, DWORD dwLen)
{
	if (0 == _memicmp(lpSrc, lpDst, dwLen))
	{
		return TRUE;
	}
	else
	{
		// �˷���������6���ַ�����
		if (12 >= dwLen)
		{
			return FALSE;
		}
		// �ж�ǰ�������ַ��Ƿ�ƥ��
		if ((lpSrc[0] != lpDst[0]) || (lpSrc[1] != lpDst[1]) ||
			(lpSrc[dwLen - 2] != lpDst[dwLen - 2]) || (lpSrc[dwLen - 1] != lpDst[dwLen - 1]))
		{
			return FALSE;
		}
		// ǰ���ַ�ƥ���ֻ����2���ַ���һ��
		DWORD dwCount = 0;
		for (DWORD i = 0; i < dwLen; i++)
		{
			if (lpSrc[i] == lpDst[i])
			{
				dwCount++;
			}
		}
		if (2 < (dwLen - dwCount))
		{
			return FALSE;
		}
	}
	return TRUE;
}


// ��ȡ�ļ�����ƫ��
BOOL FileContentOffset(HANDLE hFile, WORD wSizeOfSector, BYTE bSizeOfCluster, LARGE_INTEGER liMFTOffset, LARGE_INTEGER liRootOffset)
{
	BYTE bBuffer[1024] = { 0 };
	DWORD dwRead = 0;
	LARGE_INTEGER liContenOffset = liRootOffset;

	// ��ȡĿ¼�����ݣ���СΪ1KB
	::SetFilePointer(hFile, liRootOffset.LowPart, &liRootOffset.HighPart, FILE_BEGIN);
	::ReadFile(hFile, bBuffer, 1024, &dwRead, NULL);
	// ��ȡ��һ�����Ե�ƫ��
	WORD wAttributeOffset MAKEWORD(bBuffer[0x14], bBuffer[0x15]);
	// �����ļ�Ŀ¼������
	DWORD dwAttribute = 0;
	DWORD dwSizeOfAttribute = 0;
	while (TRUE)
	{
		// ��ȡ��ǰ����
		dwAttribute = MAKELONG(MAKEWORD(bBuffer[wAttributeOffset], bBuffer[wAttributeOffset + 1]),
			MAKEWORD(bBuffer[wAttributeOffset + 2], bBuffer[wAttributeOffset + 3]));

		// �ж�����
		if (0x80 == dwAttribute)
		{
			// ��ȡƫ��0x8��1�ֽڣ��ж��Ƿ��ǳ�פ����
			BYTE bFlag = bBuffer[wAttributeOffset + 0x8];
			if (0 == bFlag)        // ��פ
			{
				// ��ȡƫ��0x14��2�ֽڣ��������ݵ�ƫ��
				WORD wContenOffset = MAKEWORD(bBuffer[wAttributeOffset + 0x14], bBuffer[wAttributeOffset + 0x15]);
				liContenOffset.QuadPart = liContenOffset.QuadPart + wAttributeOffset + wContenOffset;
				printf("File Content Offset:0x%llx\n\n", liContenOffset.QuadPart);
			}
			else                  // �ǳ�פ
			{
				// ��ȡƫ��0x20��2�ֽڣ��������������б�ƫ��
				DWORD dwCount = 0;
				DWORD dwClusterOffet = 0;
				// ��ȡ�����ŵ�ƫ��
				WORD wIndxOffset = MAKEWORD(bBuffer[wAttributeOffset + 0x20], bBuffer[wAttributeOffset + 0x21]);
				// ��ȡData Run List
				while (TRUE)
				{
					BYTE bTemp = bBuffer[wAttributeOffset + wIndxOffset + dwCount];
					// ��ȡData Run List,�ֽⲢ����Data Run�е���Ϣ
					BYTE bHi = bTemp >> 4;
					BYTE bLo = bTemp & 0x0F;
					if (0x0F == bHi || 0x0F == bLo || 0 == bHi || 0 == bLo)
					{
						break;
					}
					LARGE_INTEGER liDataRunSize, liDataRunOffset;
					liDataRunSize.QuadPart = 0;
					liDataRunOffset.QuadPart = 0;
					for (DWORD i = bLo; i > 0; i--)
					{
						liDataRunSize.QuadPart = liDataRunSize.QuadPart << 8;
						liDataRunSize.QuadPart = liDataRunSize.QuadPart | bBuffer[wAttributeOffset + wIndxOffset + dwCount + i];
					}
					for (DWORD i = bHi; i > 0; i--)
					{
						liDataRunOffset.QuadPart = liDataRunOffset.QuadPart << 8;
						liDataRunOffset.QuadPart = liDataRunOffset.QuadPart | bBuffer[wAttributeOffset + wIndxOffset + dwCount + bLo + i];
					}
					// ע�������һ��Data Run���߼��غ�
					liDataRunOffset.QuadPart = liDataRunOffset.QuadPart + dwClusterOffet;
					dwClusterOffet = dwClusterOffet + liDataRunOffset.LowPart;

					// ��ʾ�߼��غźʹ�С
					liContenOffset.QuadPart = liDataRunOffset.QuadPart*wSizeOfSector*bSizeOfCluster;
					printf("File Content Offset:0x%llx\nFile Content Size:0x%llx\n", liContenOffset.QuadPart, (liDataRunSize.QuadPart*wSizeOfSector*bSizeOfCluster));

					// ������һ��Data Run Listƫ��
					dwCount = dwCount + bLo + bHi + 1;
				}
			}
		}
		else if (0xFFFFFFFF == dwAttribute)
		{
			break;
		}

		// ��ȡ��ǰ���ԵĴ�С
		dwSizeOfAttribute = MAKELONG(MAKEWORD(bBuffer[wAttributeOffset + 4], bBuffer[wAttributeOffset + 5]),
			MAKEWORD(bBuffer[wAttributeOffset + 6], bBuffer[wAttributeOffset + 7]));
		// ������һ���Ե�ƫ��
		wAttributeOffset = wAttributeOffset + dwSizeOfAttribute;
	}

	return TRUE;
}