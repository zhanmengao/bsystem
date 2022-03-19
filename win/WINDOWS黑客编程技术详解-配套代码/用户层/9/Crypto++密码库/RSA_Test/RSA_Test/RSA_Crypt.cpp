#include "stdafx.h"
#include "RSA_Crypt.h"


// ����RSA��Կ��
/*
	����void GenerateRSAKey(unsigned int keyLength, const char *privFilename, const char *pubFilename, const char *seed, DWORD dwSeedLength)������������Կ�͹�Կ�ļ�.
	����keyLength����Կ����, PrivFilename�Ǵ����Կ���ļ���, pubFilename�Ǵ�Ź�Կ���ļ���, seedʱ������Կ������, dwSeedLength��seedʱ������Կ�����ӳ���.
	�ú���:
	1. ��������RandomPool�ķ���Put()��������seed��byte��α�����;
	2. RSAES_OAEP_SHA_Decryptor��һ�����ܵĹ�Կ����ϵͳ���ļ�rsa.h �����¶��壺
	   typedef RSAES<OAEP<SHA> >::Decryptor RSAES_OAEP_SHA_Decryptor; �������������ǰ�������α���������Կ����keyLength�����ܵ���Կ;
	3. Ȼ��ͨ����FileSink���ļ�szPrivateKeyFileNameʵ�����л���������HexEncoder����ת��Ϊʮ������;
	4. ����÷���DEREncode()�����洦��õ��������д���ļ�.
	�����͵õ�˽Կ�������Կ�ļ��ˡ�
	������Կ�ļ��ķ����Ͳ���˽Կ��Կ�ļ���ͬ�ĵط�����ʹ����RSAES_OAEP_SHA_Encryptor��һ�����ܵĹ�Կ����ϵͳ, ���ļ�rsa.h �����¶��壺
	typedef RSAES<OAEP<SHA> >::Encryptor RSAES_OAEP_SHA_Encryptor; ���������������Կ����ϵͳpriv��������Ӧ��Կ.
*/
BOOL GenerateRSAKey(DWORD dwRSAKeyLength, char *pszPrivateKeyFileName, char *pszPublicKeyFileName, BYTE *pSeed, DWORD dwSeedLength)
{
	RandomPool randPool;
	randPool.Put(pSeed, dwSeedLength);

	// ����RSA˽Կ
	RSAES_OAEP_SHA_Decryptor priv(randPool, dwRSAKeyLength);
	HexEncoder privFile(new FileSink(pszPrivateKeyFileName));	// ���ļ�ʵ�����л�����
	
	priv.DEREncode(privFile);									
	privFile.MessageEnd();

	// ����RSA��Կ
	RSAES_OAEP_SHA_Encryptor pub(priv);
	HexEncoder pubFile(new FileSink(pszPublicKeyFileName));		// ���ļ�ʵ�����л�����

	pub.DEREncode(pubFile);										// д�������pub���ļ�����pubFile��
	pubFile.MessageEnd();

	return TRUE;
}


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
string RSA_Encrypt_ByFile(char *pszOriginaString, char *pszPublicKeyFileName, BYTE *pSeed, DWORD dwSeedLength)
{
	RandomPool randPool;
	randPool.Put(pSeed, dwSeedLength);

	FileSource pubFile(pszPublicKeyFileName, TRUE, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubFile);
	
	// ����
	string strEncryptString;
	StringSource(pszOriginaString, TRUE, new PK_EncryptorFilter(randPool, pub, new HexEncoder(new StringSink(strEncryptString))));

	return strEncryptString;
}


// RSA�����ַ���
/*
	���ܺ����Ļ������̸����ܺ����Ļ������̲�࣬��ʹ���˼�����ͬ���࣬������Щ������ܺ����Ķ�Ӧ��Ĺ�������Եģ�������������ԾͲ�����Խ���
*/
string RSA_Decrypt_ByFile(char *pszEncryptString, char *pszPrivateKeyFileName)
{
	FileSource privFile(pszPrivateKeyFileName, TRUE, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privFile);

	string strDecryptString;
	StringSource(pszEncryptString, TRUE, new HexDecoder(new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(strDecryptString))));

	return strDecryptString;
}


RandomPool & GlobalRNG()
{
	static RandomPool randomPool;

	return randomPool;
}


// RSA�����ַ���
string RSA_Encrypt_ByMem(char *pszOriginaString, char *pszMemPublicKey, BYTE *pSeed, DWORD dwSeedLength)
{
	RandomPool randPool;
	randPool.Put(pSeed, dwSeedLength);

	StringSource pubStr(pszMemPublicKey, TRUE, new HexDecoder);
	RSAES_OAEP_SHA_Encryptor pub(pubStr);

	// ����
	string strEncryptString;
	StringSource(pszOriginaString, TRUE, new PK_EncryptorFilter(randPool, pub, new HexEncoder(new StringSink(strEncryptString))));

	return strEncryptString;
}


// RSA�����ַ���
string RSA_Decrypt_ByMem(char *pszEncryptString, char *pszMemPrivateKey)
{
	StringSource privStr(pszMemPrivateKey, TRUE, new HexDecoder);
	RSAES_OAEP_SHA_Decryptor priv(privStr);

	string strDecryptString;
	StringSource(pszEncryptString, TRUE, new HexDecoder(new PK_DecryptorFilter(GlobalRNG(), priv, new StringSink(strDecryptString))));

	return strDecryptString;
}