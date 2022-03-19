#include"ResourcesUtils.h"
#include<stdio.h>
void main()
{
	char szBuffer[50] = "i am a gooooooooood student";
	DWORD dwBufferLength = strlen(szBuffer);
	BYTE* pCompressData = NULL;
	DWORD dwCompressDataLength = 0;
	BYTE* pUncompressData = NULL;
	DWORD dwUncompressDataLength = 0;

	NTSTATUS flag = ResourcesUtils::Compress((BYTE *)szBuffer, dwBufferLength, &pCompressData, &dwCompressDataLength);
	NTSTATUS flag2 = ResourcesUtils::UnCompress(pCompressData, dwCompressDataLength, &pUncompressData, &dwUncompressDataLength);
	if (flag == 0 && flag2 == 0)
	{
		for (DWORD i = 0; i < dwBufferLength; ++i)
		{
			printf("%X ", szBuffer[i]);
		}
		printf("\n");
		for (DWORD i = 0; i < dwCompressDataLength; ++i)
		{
			printf("%X ", pCompressData[i]);
		}
		printf("\n");
		for (DWORD i = 0; i < dwUncompressDataLength; ++i)
		{
			printf("%X ", pUncompressData[i]);
		}
	}
	printf("\n pUncompressData : %s \n", pUncompressData);
	if (pCompressData != NULL)
	{
		delete[] pCompressData;
		pCompressData = NULL;
	}
	if (pUncompressData != NULL)
	{
		delete[] pUncompressData;
		pUncompressData = NULL;
	}
}