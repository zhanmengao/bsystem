#include <iostream>
using namespace std;
#include "openssl/evp.h"  //�������opensslͷ�ļ���λ��/usr/local/ssl/include/openssl/evp.h
int main(int argc, char *argv[])
{
	char sz[] = "Hello, OpenSSL!";	
	cout << sz << endl;
	OpenSSL_add_all_algorithms();  //��������SSL�㷨�����������openssl���еĺ���
	return 0;
}
