// Zlib_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "zlib_compress.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BOOL bRet = FALSE;
	BYTE *pCompressData = NULL;
	DWORD dwCompressDataSize = 0;
	BYTE *pUncompressData = NULL;
	DWORD dwUncompressDataSize = 0;


	// ѹ���ļ�
	bRet = Zlib_CompressData("520.exe", &pCompressData, &dwCompressDataSize);
	if (FALSE == bRet)
	{
		return 1;
	}

	// ����ѹ������Ϊ�ļ�
	bRet = SaveToOriginalFile("520.myzip", pCompressData, dwCompressDataSize);
	if (FALSE == bRet)
	{
		return 2;
	}

	// ��ѹ��ѹ���ļ�
	bRet = Zlib_UncompressData("520.myzip", &pUncompressData, &dwUncompressDataSize);
	if (FALSE == bRet)
	{
		return 3;
	}

	// ����ѹ������Ϊ�ļ�
	bRet = SaveToOriginalFile("520_Uncompress.exe", pUncompressData, dwUncompressDataSize);
	if (FALSE == bRet)
	{
		return 4;
	}
	
	// �ͷ��ڴ�
	delete []pUncompressData;
	pUncompressData = NULL;
	delete []pCompressData;
	pCompressData = NULL;

	system("pause");
	return 0;
}

