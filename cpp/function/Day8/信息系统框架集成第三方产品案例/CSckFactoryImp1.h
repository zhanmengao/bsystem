
#pragma  once

#include <iostream>
using namespace std;
#include "CSocketProtocol.h"

class  CSckFactoryImp1 : public CSocketProtocol
{
public:

	//�ͻ��˳�ʼ�� ��ȡhandle����
	virtual int cltSocketInit( /*out*/); 

	//�ͻ��˷�����
	virtual int cltSocketSend( unsigned char *buf /*in*/,  int buflen /*in*/); 

	//�ͻ����ձ���
	virtual int cltSocketRev( unsigned char *buf /*in*/, int *buflen /*in out*/);

	//�ͻ����ͷ���Դ
	virtual int cltSocketDestory();

private:
	unsigned char *p;
	int len ;
};


