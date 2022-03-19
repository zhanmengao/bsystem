
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#include "CSocketProtocol.h"
#include "CSckFactoryImp1.h"
#include "CSckFactoryImp2.h"

//�����������,���ʵ�����
int SckSendAndRec01(CSocketProtocol *sp, unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	int ret = 0;
	ret = sp->cltSocketInit();
	if (ret != 0)
	{
		goto End;
	}

	ret = sp->cltSocketSend(in, inlen);
	if (ret != 0)
	{
		goto End;
	}
	
	ret = sp->cltSocketRev(out, outlen);
	if (ret != 0)
	{
		goto End;
	}

End:
	ret = sp->cltSocketDestory();
	return 0;
}


//дһ�����
int main011()
{
	int ret = 0;
	unsigned char in[4096];
	int inlen;
	unsigned char out[4096];
	int outlen = 0;

	strcpy((char *)in, "aadddddddddddaaaaaaaaaaa");
	inlen = 9;


	CSocketProtocol *sp = NULL;
	//sp = new CSckFactoryImp1

	sp = new CSckFactoryImp2; 

	ret = SckSendAndRec01(sp, in, inlen, out, &outlen);       //���һ��ͨ��
	if (ret != 0)
	{
		printf("func SckSendAndRec() err:%d \n", ret);
		return ret;
	}
	delete sp; //��ͨ������ָ�� �ͷ����е�����������Դ ..
	return ret;
}