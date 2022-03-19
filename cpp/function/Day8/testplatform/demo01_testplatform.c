#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "socketclientdll.h"
#pragma comment(lib,"testplatform/socketclient.lib")
#define _CRT_SECURE_NO_WARNINGS
//typedef int (*EncData)(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen);

//是厂商的加密函数
int myEncData(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen)
{
	memcpy(outData, "111111111111111111111111111111111", 10);
	*outDataLen = 10;
	return 0;
}

int main1()
{
	int				rv = 0;
	void			*handle = NULL;
	unsigned char	buf[2048];
	int				buflen = 0;

	unsigned char	buf2[2048] = {0};
	int				buflen2 = 0;

	strcpy(buf, "aaaaaaaaaaaaaaaaaaaaaaaaafffffffffffffffffffffff");


	buflen = 10;
	rv = cltSocketInit(&handle);
	if (rv != 0)
	{
		printf("func cltSocketInit():%d", rv);
		return rv;
	}
	
	// cvtres.exe
	///////--方法二///////////////////////////////////////////////////////////////////
	rv = cltSocketSetEncFunc(handle, myEncData, NULL, 0);
	if (rv != 0)
	{
		printf("func cltSocketInit():%d", rv);
		goto End;
	}
	
	rv = cltSocketSend(handle, buf,  buflen);
	if (rv != 0)
	{
		printf("func cltSocketSend():%d", rv);
		goto End;
	}
	///////--方法二///////////////////////////////////////////////////////////////////
	
	/*方法1 
	rv = cltSocket_EncAndSend(handle, buf,  buflen, myEncData, NULL, 0);
	if (rv != 0)
	{
		printf("func cltSocketSend_enc():%d", rv);
		goto End;
	}
	*/
	


	rv = cltSocketRev(handle, buf2 , &buflen2);
	if (rv != 0)
	{
		printf("func cltSocketRev():%d", rv);
		goto End;
	}
	printf("\n%s", buf2);

End:

	rv = cltSocketDestory(handle);
	if (rv != 0)
	{
		printf("func cltSocketDestory():%d", rv);
		return rv;
	}

	printf("hello....\n");
	
	//system("pause");

	return 0;
}

