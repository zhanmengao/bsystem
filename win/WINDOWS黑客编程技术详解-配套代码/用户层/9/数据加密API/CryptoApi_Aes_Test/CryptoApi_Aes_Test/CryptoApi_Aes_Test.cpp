// CryptoApi_Aes_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>


void ShowError(char *pszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error[0x%x]\n", pszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


// AES����
BOOL AesEncrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// ��ȡCSP���
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// ����HASH����
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			ShowError("CryptCreateHash");
			break;
		}

		// ����Կ����HASH����
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptHashData");
			break;
		}

		// ʹ��HASH��������Կ
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptDeriveKey");
			break;
		}

		// ��������
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			ShowError("CryptEncrypt");
			break;
		}

	} while (FALSE);

	// �ر��ͷ�
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		::CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// AES����
BOOL AesDecrypt(BYTE *pPassword, DWORD dwPasswordLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTHASH hCryptHash = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// ��ȡCSP���
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// ����HASH����
		bRet = ::CryptCreateHash(hCryptProv, CALG_MD5, NULL, 0, &hCryptHash);
		if (FALSE == bRet)
		{
			ShowError("CryptCreateHash");
			break;
		}

		// ����Կ����HASH����
		bRet = ::CryptHashData(hCryptHash, pPassword, dwPasswordLength, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptHashData");
			break;
		}

		// ʹ��HASH��������Կ
		bRet = ::CryptDeriveKey(hCryptProv, CALG_AES_128, hCryptHash, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptDeriveKey");
			break;
		}

		// ��������
		bRet = ::CryptDecrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength);
		if (FALSE == bRet)
		{
			ShowError("CryptDecrypt");
			break;
		}

	} while (FALSE);

	// �ر��ͷ�
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptHash)
	{
		::CryptDestroyHash(hCryptHash);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


int _tmain(int argc, _TCHAR* argv[])
{
	BYTE pData[MAX_PATH] = { 0 };
	DWORD dwDataLength = 0, dwBufferLength = MAX_PATH;
	DWORD i = 0;

	::RtlZeroMemory(pData, dwBufferLength);
	::lstrcpy((char *)pData, "What is your name? DemonGan");
	dwDataLength = 1 + ::lstrlen((char *)pData);

	printf("Text[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x ", pData[i]);
	}
	printf("\n\n");

	// AES ����
	AesEncrypt((BYTE *)"DemonGanDemonGan", 16, pData, dwDataLength, dwBufferLength);
	printf("AES Encrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x ", pData[i]);
	}
	printf("\n\n");
	
	// AES ����
	AesDecrypt((BYTE *)"DemonGanDemonGan", 16, pData, dwDataLength, dwBufferLength);
	printf("AES Decrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x ", pData[i]);
	}
	printf("\n\n");

	system("pause");
	return 0;
}

