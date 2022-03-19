
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#include "CSocketProtocol.h"
#include "CSckFactoryImp1.h"
#include "CSckFactoryImp2.h"

#include "CEncDesProtocol.h"
#include "HwEncDec.h"

//面向抽象类编程,框架实现完毕
int SckSendAndRec(CSocketProtocol *sp, unsigned char *in, int inlen, unsigned char *out, int *outlen)
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


//面向抽象类编程,框架实现完毕
//c函数
int SckSendAndRec_EncDec(CSocketProtocol *sp, CEncDesProtocol *ed, unsigned char *in, int inlen, unsigned char *out, int *outlen)
{
	int ret = 0;
	unsigned char data[4096];
	int datalen = 0;

	ret = sp->cltSocketInit();
	if (ret != 0)
	{
		goto End;
	}

	ret = ed->EncData(in,inlen, data, &datalen);
	if (ret != 0)
	{
		goto End;
	}
	ret = sp->cltSocketSend(data, datalen); //发送数据之前对数据加密 ..
	if (ret != 0)
	{
		goto End;
	}

	ret = sp->cltSocketRev(data, &datalen); //收到的数据是密文,需要进行解密
	if (ret != 0)
	{
		goto End;
	}
	ret = ed->DecData(data, datalen, out, outlen );
	if (ret != 0)
	{
		goto End;
	}

End:
	ret = sp->cltSocketDestory();
	return 0;
}

//写一个框架
int main022()
{
	int ret = 0;
	unsigned char in[4096];
	int inlen;
	unsigned char out[4096];
	int outlen = 0;

	strcpy((char *)in, "aadddddddddddaaaaaaaaaaa");
	inlen = 9;


	CSocketProtocol *sp = NULL;                    //传输基类
	CEncDesProtocol *ed = NULL;                    //加密基类

	//sp = new CSckFactoryImp1

	sp = new CSckFactoryImp2; //
	ed = new HwEncDec;

	ret = SckSendAndRec_EncDec(sp, ed, in, inlen, out, &outlen);
	if (ret != 0)
	{
		printf("func SckSendAndRec() err:%d \n", ret);
		return ret;
	}
	delete sp; //想通过父类指针 释放所有的子类对象的资源 ..

	return ret;
}