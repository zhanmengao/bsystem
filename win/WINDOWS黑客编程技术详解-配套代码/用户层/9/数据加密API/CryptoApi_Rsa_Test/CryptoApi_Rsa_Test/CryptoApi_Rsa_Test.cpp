// CryptoApi_Rsa_Test.cpp : �������̨Ӧ�ó������ڵ㡣
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


// ���ɹ�Կ��˽Կ
BOOL GenerateKey(BYTE **ppPublicKey, DWORD *pdwPublicKeyLength, BYTE **ppPrivateKey, DWORD *pdwPrivateKeyLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;
	BYTE *pPublicKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE *pPrivateKey = NULL;
	DWORD dwPrivateKeyLength = 0;

	do
	{
		// ��ȡCSP���
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// ���ɹ�˽��Կ��
		bRet = ::CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptGenKey");
			break;
		}

		// ��ȡ��Կ��Կ�ĳ��Ⱥ�����
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, NULL, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			ShowError("CryptExportKey");
			break;
		}
		pPublicKey = new BYTE[dwPublicKeyLength];
		::RtlZeroMemory(pPublicKey, dwPublicKeyLength);
		bRet = ::CryptExportKey(hCryptKey, NULL, PUBLICKEYBLOB, 0, pPublicKey, &dwPublicKeyLength);
		if (FALSE == bRet)
		{
			ShowError("CryptExportKey");
			break;
		}

		// ��ȡ˽Կ��Կ�ĳ��Ⱥ�����
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, NULL, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			ShowError("CryptExportKey");
			break;
		}
		pPrivateKey = new BYTE[dwPrivateKeyLength];
		::RtlZeroMemory(pPrivateKey, dwPrivateKeyLength);
		bRet = ::CryptExportKey(hCryptKey, NULL, PRIVATEKEYBLOB, 0, pPrivateKey, &dwPrivateKeyLength);
		if (FALSE == bRet)
		{
			ShowError("CryptExportKey");
			break;
		}

		// ��������
		*ppPublicKey = pPublicKey;
		*pdwPublicKeyLength = dwPublicKeyLength;
		*ppPrivateKey = pPrivateKey;
		*pdwPrivateKeyLength = dwPrivateKeyLength;

	} while (FALSE);

	// �ͷŹر�
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// ��Կ��������
BOOL RsaEncrypt(BYTE *pPublicKey, DWORD dwPublicKeyLength, BYTE *pData, DWORD &dwDataLength, DWORD dwBufferLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// ��ȡCSP���
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// ���빫Կ
		bRet = ::CryptImportKey(hCryptProv, pPublicKey, dwPublicKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptImportKey");
			break;
		}

		// ��������
		bRet = ::CryptEncrypt(hCryptKey, NULL, TRUE, 0, pData, &dwDataLength, dwBufferLength);
		if (FALSE == bRet)
		{
			ShowError("CryptImportKey");
			break;
		}

	} while (FALSE);

	// �ͷŲ��ر�
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}


// ˽Կ��������
BOOL RsaDecrypt(BYTE *pPrivateKey, DWORD dwProvateKeyLength, BYTE *pData, DWORD &dwDataLength)
{
	BOOL bRet = TRUE;
	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hCryptKey = NULL;

	do
	{
		// ��ȡCSP���
		bRet = ::CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
		if (FALSE == bRet)
		{
			ShowError("CryptAcquireContext");
			break;
		}

		// ����˽Կ
		bRet = ::CryptImportKey(hCryptProv, pPrivateKey, dwProvateKeyLength, NULL, 0, &hCryptKey);
		if (FALSE == bRet)
		{
			ShowError("CryptImportKey");
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

	// �ͷŲ��ر�
	if (hCryptKey)
	{
		::CryptDestroyKey(hCryptKey);
	}
	if (hCryptProv)
	{
		::CryptReleaseContext(hCryptProv, 0);
	}

	return bRet;
}



int _tmain(int argc, _TCHAR* argv[])
{
	BYTE *pPublicKey = NULL;
	DWORD dwPublicKeyLength = 0;
	BYTE *pPrivateKey = NULL;
	DWORD dwPrivateKeyLength = 0;
	BYTE *pData = NULL;
	DWORD dwDataLength = 0;
	DWORD dwBufferLength = 4096;
	DWORD i = 0;

	pData = new BYTE[dwBufferLength];
	if (NULL == pData)
	{
		return 1;
	}
	::RtlZeroMemory(pData, dwBufferLength);
	::lstrcpy((char *)pData, "What is your name? DemonGan");
	dwDataLength = 1 + ::lstrlen((char *)pData);
	printf("Text[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\n\n");

	// ���ɹ�Կ��˽Կ
	GenerateKey(&pPublicKey, &dwPublicKeyLength, &pPrivateKey, &dwPrivateKeyLength);
	printf("Public Key[%d]\n", dwPublicKeyLength);
	for (i = 0; i < dwPublicKeyLength; i++)
	{
		printf("%.2x", pPublicKey[i]);
	}
	printf("\n");
	printf("Private Key[%d]\n", dwPrivateKeyLength);
	for (i = 0; i < dwPrivateKeyLength; i++)
	{
		printf("%.2x", pPrivateKey[i]);
	}
	printf("\n\n");

	// ��Կ����
	RsaEncrypt(pPublicKey, dwPublicKeyLength, pData, dwDataLength, dwBufferLength);
	printf("RSA Encrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\n\n");

	// ˽Կ����
	RsaDecrypt(pPrivateKey, dwPrivateKeyLength, pData, dwDataLength);
	printf("RSA Decrypt[%d]\n", dwDataLength);
	for (i = 0; i < dwDataLength; i++)
	{
		printf("%x", pData[i]);
	}
	printf("\n\n");

	// �ͷ�
	if (pData)
	{
		delete[]pData;
		pData = NULL;
	}
	if (pPrivateKey)
	{
		delete[]pPrivateKey;
		pPrivateKey = NULL;
	}
	if (pPublicKey)
	{
		delete[]pPublicKey;
		pPublicKey = NULL;
	}

	system("pause");
	return 0;
}

