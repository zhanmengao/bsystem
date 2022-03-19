#include "stdafx.h"
#include "AES_Crypt.h"


// ��Կ����
// �����AES��Կ��AES��Կ����
BOOL GenerateAESKey(BYTE *pAESKey, DWORD *pdwAESKeyLength, DWORD dwBufferSize)
{
	srand(time(NULL));

	if (AES::MIN_KEYLENGTH > *pdwAESKeyLength)
	{
		*pdwAESKeyLength = AES::MIN_KEYLENGTH;
	}
	else if (AES::MAX_KEYLENGTH < *pdwAESKeyLength)
	{
		*pdwAESKeyLength = AES::MAX_KEYLENGTH;
	}
	

	// ��Կ���ȴ��ڻ�����
	if (dwBufferSize < *pdwAESKeyLength)
	{
		return FALSE;
	}

	// ���������Կ(��Сд��ĸ�����֡��ַ��ȿ���ʾ�ַ�)
	// 33 - 126
	int i = 0;
	::RtlZeroMemory(pAESKey, dwBufferSize);
	for (i = 0; i < *pdwAESKeyLength; i++)
	{
		pAESKey[i] = 33 + (rand() % 94);
	}

	return TRUE;
}


// ����
// ���룺ԭ�����ݡ�ԭ�����ݳ��ȡ���Կ���ݡ���Կ���ݳ���
// ������������ݡ��������ݳ���
BOOL AES_Encrypt(BYTE *pOriginalData, DWORD dwOriginalDataSize, BYTE *pAESKey, DWORD dwAESKeySize, BYTE **ppEncryptData, DWORD *pdwEncryptData)
{
	// ������
	AESEncryption aesEncryptor;			
	// ����ԭ�����ݿ�
	unsigned char inBlock[AES::BLOCKSIZE];	
	// ���ܺ��������ݿ�
	unsigned char outBlock[AES::BLOCKSIZE];	
	// �����趨ȫΪ0
	unsigned char xorBlock[AES::BLOCKSIZE];	

	DWORD dwOffset = 0;
	BYTE *pEncryptData = NULL;
	DWORD dwEncryptDataSize = 0;

	// ����ԭ�ĳ���, ���� 128λ �� 16�ֽ� ����, ������ ���0 ����
	// ��
	DWORD dwQuotient = dwOriginalDataSize / AES::BLOCKSIZE;  
	// ����
	DWORD dwRemaind = dwOriginalDataSize % AES::BLOCKSIZE;		
	if (0 != dwRemaind)
	{
		dwQuotient++;
	}

	// ���붯̬�ڴ�
	dwEncryptDataSize = dwQuotient * AES::BLOCKSIZE;
	pEncryptData = new BYTE[dwEncryptDataSize];
	if (NULL == pEncryptData)
	{
		return FALSE;
	}

	// ������Կ
	aesEncryptor.SetKey(pAESKey, dwAESKeySize);

	do
	{
		// ����
		::RtlZeroMemory(inBlock, AES::BLOCKSIZE);
		::RtlZeroMemory(xorBlock, AES::BLOCKSIZE);
		::RtlZeroMemory(outBlock, AES::BLOCKSIZE);

		// ��ȡ���ܿ�
		if (dwOffset <= (dwOriginalDataSize - AES::BLOCKSIZE))
		{
			::RtlCopyMemory(inBlock, (PVOID)(pOriginalData + dwOffset), AES::BLOCKSIZE);
		}
		else
		{
			::RtlCopyMemory(inBlock, (PVOID)(pOriginalData + dwOffset), (dwOriginalDataSize - dwOffset));
		}

		// ����
		aesEncryptor.ProcessAndXorBlock(inBlock, xorBlock, outBlock);

		// ����
		::RtlCopyMemory((PVOID)(pEncryptData + dwOffset), outBlock, AES::BLOCKSIZE);

		// ��������
		dwOffset = dwOffset + AES::BLOCKSIZE;
		dwQuotient--;
	} while (0 < dwQuotient);

	// ��������
	*ppEncryptData = pEncryptData;
	*pdwEncryptData = dwEncryptDataSize;

	return TRUE;
}


// ����
// ���룺�������ݡ��������ݳ��ȡ���Կ���ݡ���Կ���ݳ���
// ��������ܺ��������ݡ����ܺ��������ݳ���
BOOL AES_Decrypt(BYTE *pEncryptData, DWORD dwEncryptData, BYTE *pAESKey, DWORD dwAESKeySize, BYTE **ppDecryptData, DWORD *pdwDecryptData)
{
	// ������
	AESDecryption aesDecryptor;				
	// �����������ݿ�
	unsigned char inBlock[AES::BLOCKSIZE];		
	// ���ܺ���������ݿ�
	unsigned char outBlock[AES::BLOCKSIZE];			
	// �����趨ȫΪ0
	unsigned char xorBlock[AES::BLOCKSIZE];						
	DWORD dwOffset = 0;
	BYTE *pDecryptData = NULL;
	DWORD dwDecryptDataSize = 0;

	// �������ĳ���, ���� 128λ �� 16�ֽ� ����, ���������0����
	// ��
	DWORD dwQuotient = dwEncryptData / AES::BLOCKSIZE;
	// ����
	DWORD dwRemaind = dwEncryptData % AES::BLOCKSIZE;		
	if (0 != dwRemaind)
	{
		dwQuotient++;
	}

	// ���붯̬�ڴ�
	dwDecryptDataSize = dwQuotient * AES::BLOCKSIZE;
	pDecryptData = new BYTE[dwDecryptDataSize];
	if (NULL == pDecryptData)
	{
		return FALSE;
	}

	// ������Կ
	aesDecryptor.SetKey(pAESKey, dwAESKeySize);

	do
	{
		// ����
		::RtlZeroMemory(inBlock, AES::BLOCKSIZE);
		::RtlZeroMemory(xorBlock, AES::BLOCKSIZE);
		::RtlZeroMemory(outBlock, AES::BLOCKSIZE);

		// ��ȡ���ܿ�
		if (dwOffset <= (dwDecryptDataSize - AES::BLOCKSIZE))
		{
			::RtlCopyMemory(inBlock, (PVOID)(pEncryptData + dwOffset), AES::BLOCKSIZE);
		}
		else
		{
			::RtlCopyMemory(inBlock, (PVOID)(pEncryptData + dwOffset), (dwEncryptData - dwOffset));
		}

		// ����
		aesDecryptor.ProcessAndXorBlock(inBlock, xorBlock, outBlock);

		// ����
		::RtlCopyMemory((PVOID)(pDecryptData + dwOffset), outBlock, AES::BLOCKSIZE);

		// ��������
		dwOffset = dwOffset + AES::BLOCKSIZE;
		dwQuotient--;
	} while (0 < dwQuotient);

	// ��������
	*ppDecryptData = pDecryptData;
	*pdwDecryptData = dwDecryptDataSize;

	return TRUE;
}


// ��ʾ����
// ���룺����������ݡ�����������ݳ���
void ShowData(BYTE *pData, DWORD dwSize)
{
	for (int i = 0; i < dwSize; i++)
	{
		if ((0 != i) &&
			(0 == i % 16))
		{
			printf("\n");
		}
		else if ((0 != i) &&
			(0 == i % 8))
		{
			printf(" ");
		}

		printf("%02X ", pData[i]);
	}
	printf("\n");
}