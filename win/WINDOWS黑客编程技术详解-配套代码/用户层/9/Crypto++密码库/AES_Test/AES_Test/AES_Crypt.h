#ifndef _AES_CRYPT_H_
#define _AES_CRYPT_H_


#include <time.h>
#include <Windows.h>
//*************************************************
//         crypt++���ܿ��ͷ�ļ��;�̬��
//*************************************************
#include "crypt\\include\\aes.h"
using namespace CryptoPP;         // �����ռ�

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "crypt\\lib\\x64\\debug\\cryptlib.lib")
	#else
		#pragma comment(lib, "crypt\\lib\\x86\\debug\\cryptlib.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "crypt\\lib\\x64\\release\\cryptlib.lib")
	#else
		#pragma comment(lib, "crypt\\lib\\x86\\release\\cryptlib.lib")
	#endif
#endif
//*************************************************


// ��Կ����
// �����AES��Կ��AES��Կ����
BOOL GenerateAESKey(BYTE *pAESKey, DWORD *pdwAESKeyLength, DWORD dwBufferSize);


// ����
// ���룺ԭ�����ݡ�ԭ�����ݳ��ȡ���Կ���ݡ���Կ���ݳ���
// ������������ݡ��������ݳ���
BOOL AES_Encrypt(BYTE *pOriginalData, DWORD dwOriginalDataSize, BYTE *pAESKey, DWORD dwAESKeySize, BYTE **ppEncryptData, DWORD *pdwEncryptData);

// ����
// ���룺�������ݡ��������ݳ��ȡ���Կ���ݡ���Կ���ݳ���
// ��������ܺ��������ݡ����ܺ��������ݳ���
BOOL AES_Decrypt(BYTE *pEncryptData, DWORD dwEncryptData, BYTE *pAESKey, DWORD dwAESKeySize, BYTE **ppDecryptData, DWORD *pdwDecryptData);

// ��ʾ����
// ���룺����������ݡ�����������ݳ���
void ShowData(BYTE *pData, DWORD dwSize);



#endif