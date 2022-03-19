#ifndef _MD5_TEST_H_
#define _MD5_TEST_H_


#include <Windows.h>
using namespace std;
//*************************************************
//         crypt++���ܿ��ͷ�ļ��;�̬��
//*************************************************
#include "crypt\\include\\md5.h"
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


// �����ļ��� MD5 ֵ
string CalMD5_ByFile(char *pszFileName);

// �������ݵ� MD5 ֵ
string CalMD5_ByMem(PBYTE pData, DWORD dwDataSize);


#endif