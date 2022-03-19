
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#include "CSocketProtocol.h"
#include "CSckFactoryImp1.h"
#include "CSckFactoryImp2.h"

#include "CEncDesProtocol.h"
#include "HwEncDec.h"

//抽象类在多继承中的应用
/*
class  MainOp : public CSocketProtocol, public CEncDesProtocol
{
public:
protected:
private:

};
*/

class MainOp
{
public:
	MainOp()
	{
		this->sp = NULL;
		this->ed = NULL;
	}
	MainOp(CSocketProtocol *sp, CEncDesProtocol *ed)
	{
		this->sp = sp;
		this->ed = ed;
	}
	//

	void setSp(CSocketProtocol *sp)
	{
		this->sp = sp;
	}

	void setEd(CEncDesProtocol *ed)
	{
		this->ed = ed;
	}

public:
	//面向抽象类编程,框架实现完毕
	int SckSendAndRec_EncDec3(CSocketProtocol *sp, CEncDesProtocol *ed, unsigned char *in, int inlen, unsigned char *out, int *outlen)
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

	int SckSendAndRec_EncDec3(unsigned char *in, int inlen, unsigned char *out, int *outlen)
	{
		int ret = 0;
		unsigned char data[4096];
		int datalen = 0;


		ret = this->sp->cltSocketInit();
		if (ret != 0)
		{
			goto End;
		}

		ret = this->ed->EncData(in,inlen, data, &datalen);
		if (ret != 0)
		{
			goto End;
		}
		ret = this->sp->cltSocketSend(data, datalen); //发送数据之前对数据加密 ..
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

private:
	CSocketProtocol *sp;
	CEncDesProtocol *ed;

};

//写一个框架
int main3()
{
	int ret = 0;
	unsigned char in[4096];
	int inlen;
	unsigned char out[4096];
	int outlen = 0;

	strcpy((char *)in, "aadddddddddddaaaaaaaaaaa");
	inlen = 9;


	MainOp *myMainOp = new MainOp;

	CSocketProtocol *sp = NULL;
	CEncDesProtocol *ed = NULL;

	//sp = new CSckFactoryImp1

	sp = new CSckFactoryImp2; //
	ed = new HwEncDec;

	myMainOp->setSp(sp);
	myMainOp->setEd(ed);

	ret = myMainOp->SckSendAndRec_EncDec3(in, inlen, out, &outlen);              //将编码和加密做成成员函数
	if (ret!= 0)
	{
		printf("myMainOp SckSendAndRec_EncDec3() err\n ", ret);
	}

	delete sp;
	delete ed;
	delete myMainOp;

	return ret;
}