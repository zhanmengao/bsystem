
#pragma  once

#include <iostream>
using namespace std;
/*
Socket������
*/
class CSocketProtocol
{
public:
	CSocketProtocol()
	{
		;
	}

	virtual ~CSocketProtocol() //������������ϸ��
	{
		;
	}

	//�ͻ��˳�ʼ�� ��ȡhandle����
	virtual int cltSocketInit( /*out*/) = 0; 

	//�ͻ��˷�����
	virtual int cltSocketSend( unsigned char *buf /*in*/,  int buflen /*in*/)  = 0; 
	
	//�ͻ����ձ���
	virtual int cltSocketRev( unsigned char *buf /*in*/, int *buflen /*in out*/) = 0;

	//�ͻ����ͷ���Դ
	virtual int cltSocketDestory() = 0;

};


