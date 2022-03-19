#ifndef _RSA_CRYPT_H_
#define _RSA_CRYPT_H_


#include <Windows.h>
#include <iostream>
using namespace std;
//*************************************************
//         crypt++���ܿ��ͷ�ļ��;�̬��
//*************************************************
#include "crypt\\include\\rsa.h"
#include "crypt\\include\\randpool.h"
#include "crypt\\include\\hex.h"
#include "crypt\\include\\files.h"
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

// ����RSA��Կ��
/*
����void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename, const char *seed, DWORD dwSeedLength)������������Կ�͹�Կ�ļ�.
����keyLength����Կ����, PrivFilename�Ǵ����Կ���ļ���, pubFilename�Ǵ�Ź�Կ���ļ���, seedʱ������Կ������, dwSeedLength��seedʱ������Կ�����ӳ���.
�ú���:
1. ��������RandomPool�ķ���Put()��������seed��byte��α�����;
2. RSAES_OAEP_SHA_Decryptor��һ�����ܵĹ�Կ����ϵͳ���ļ�rsa.h �����¶��壺
typedef RSAES<OAEP<SHA> >::Decryptor RSAES_OAEP_SHA_Decryptor; �������������ǰ�������α���������Կ����keyLength�����ܵ���Կ;
3. Ȼ��ͨ����FileSink���ļ�pubFilenameʵ�����л���������HexEncoder����ת��Ϊʮ������;
4. ����÷���DEREncode()�����洦��õ��������д���ļ�.
�����͵õ���Կ�������Կ�ļ��ˡ�
������Կ�ļ��ķ����Ͳ�����Կ�ļ���ͬ�ĵط�����ʹ����RSAES_OAEP_SHA_Encryptor��һ�����ܵĹ�Կ����ϵͳ, ���ļ�rsa.h �����¶��壺
typedef RSAES<OAEP<SHA> >::Encryptor RSAES_OAEP_SHA_Encryptor; ���������������Կ����ϵͳpriv��������Ӧ��Կ.
*/
BOOL GenerateRSAKey(DWORD dwRSAKeyLength, char *pszPrivateKeyFileName, char *pszPublicKeyFileName, BYTE *pSeed, DWORD dwSeedLength);


// RSA�����ַ���
/*
���ܺ���string RSA_Encrypt(char *pszOriginaString, char *pszPublicKeyFileName, BYTE *pSeed, DWORD dwSeedLength) ��pszPublicKeyFileName�ǹ�Կ�ļ���pSeed�Ǽ������ӣ�pszOriginaString��Ҫ���ܵ��ַ�����
��������Ļ��������ǣ�
1. ��������RandomPool������seed���÷���Put()����α�������Seed������ȡ;;
2. ����FileSource�Թ�Կ�ļ�pubFilename����һ����ת��������ʱ����������������ʮ������ת��Ϊbyte��;
3. Ȼ����FileSource�Ķ���pubFile ʵ������Կ����ϵͳRSAES_OAEP_SHA_Encryptor���ɶ���pub;
4. ����StringSink ��outstr��ӵ�һ��String���󣬽�����HexEncoder���������ת��Ϊʮ������;
5. Ȼ����α�����randPool����Կ����ϵͳpub��ʮ�����Ƶ�String����ʵ����һ����Կ������ܵĹ�����������������������ַ���message���м��ܰѽ���ŵ�ʮ�����Ƶ��ַ���result�����������˶��ַ����ļ��ܡ�
*/
string RSA_Encrypt_ByFile(char *pszOriginaString, char *pszPublicKeyFileName, BYTE *pSeed, DWORD dwSeedLength);


// RSA�����ַ���
/*
���ܺ����Ļ������̸����ܺ����Ļ������̲�࣬��ʹ���˼�����ͬ���࣬������Щ������ܺ����Ķ�Ӧ��Ĺ�������Եģ�������������ԾͲ�����Խ���
*/
string RSA_Decrypt_ByFile(char *pszEncryptString, char *pszPrivateKeyFileName);


// ����ȫ���������
RandomPool & GlobalRNG();


// RSA�����ַ���
string RSA_Encrypt_ByMem(char *pszOriginaString, char *pszMemPublicKey, BYTE *pSeed, DWORD dwSeedLength);


// RSA�����ַ���
string RSA_Decrypt_ByMem(char *pszEncryptString, char *pszMemPrivateKey);




#endif
