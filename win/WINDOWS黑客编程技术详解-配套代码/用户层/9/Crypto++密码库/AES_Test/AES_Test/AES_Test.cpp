// AES_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "AES_Crypt.h"


int _tmain(int argc, _TCHAR* argv[])
{
	BYTE *pEncryptData = NULL;
	DWORD dwEncryptDataSize = 0;
	BYTE *pDecryptData = NULL;
	DWORD dwDecryptDataSize = 0;
	char szOriginalData[] = "I am DemonGan";
	char szAESKey[] = "DemonGanDemonGan";
	BOOL  bRet = FALSE;

	// ����
	bRet = AES_Encrypt((BYTE *)szOriginalData, (1 + ::lstrlen(szOriginalData)), (BYTE *)szAESKey, ::lstrlen(szAESKey), &pEncryptData, &dwEncryptDataSize);
	if (FALSE == bRet)
	{
		return 1;
	}

	// ����
	bRet = AES_Decrypt(pEncryptData, dwEncryptDataSize, (BYTE *)szAESKey, ::lstrlen(szAESKey), &pDecryptData, &dwDecryptDataSize);
	if (FALSE == bRet)
	{
		return 2;
	}

	// ��ʾ
	printf("ԭ������:\n");
	ShowData((BYTE *)szOriginalData, (1 + ::lstrlen(szOriginalData)));
	printf("��������:\n");
	ShowData(pEncryptData, dwEncryptDataSize);
	printf("���ܺ�����:\n");
	ShowData(pDecryptData, dwDecryptDataSize);

	// �ͷ��ڴ�
	delete[]pEncryptData;
	pEncryptData = NULL;
	delete[]pDecryptData;
	pDecryptData = NULL;

	system("pause");
	return 0;
}

