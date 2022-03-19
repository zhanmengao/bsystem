#ifndef _SHA1_TEST_H_
#define _SHA1_TEST_H_


#include <Windows.h>
using namespace std;
//*************************************************
//         crypt++���ܿ��ͷ�ļ��;�̬��
//*************************************************
#include "crypt\\include\\sha.h"
#include "crypt\\include\\files.h"
#include "crypt\\include\\hex.h"
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


// �����ļ��� SHA256 ֵ
string CalSHA256_ByFile(char *pszFileName);

// �������ݵ� SHA256 ֵ
string CalSHA256_ByMem(PBYTE pData, DWORD dwDataSize);


#endif