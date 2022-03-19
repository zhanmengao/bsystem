#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "des.h"

int main11()
{
	int		ret = 0;
	char *plain = "1232343444";
	int plainlen = strlen(plain);

	char plain2[4096] = {0};
	int plainlen2 = 0;

	unsigned char cryptbuf[4096] = {0};
	int cryptlen = 0;
	//用户使用的函数
	ret =  DesEnc(plain, plainlen, cryptbuf, &cryptlen);
	if (ret != 0)
	{
		printf("func DesEnc() err:%d \n");
		return ret;
	}

	//用户使用函数des解密
	ret =   DesDec(cryptbuf, cryptlen,  plain2, &plainlen2);
	if (ret != 0)
	{
		printf("func DesDec() err:%d \n");
		return ret;
	}

	if (plainlen != plainlen2)
	{
		ret = -2;
		printf("明文长度和解密后的明文长度 不一样 \n");
		return ret;
	}

	if (memcmp(plain2, plain, plainlen) == 0 )
	{
		printf("明文长度和解密后的明文长度 一样 ok \n");
	}
	else
	{
		printf("明文长度和解密后的明文长度 不一样 err \n");
	}

	system("pause");
	return ret ;
}