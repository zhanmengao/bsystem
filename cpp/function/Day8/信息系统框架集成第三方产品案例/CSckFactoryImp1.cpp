
#include <iostream>
using namespace std;

#include "CSckFactoryImp1.h"


//�ͻ��˳�ʼ�� ��ȡhandle����
 int CSckFactoryImp1::cltSocketInit( /*out*/)
 {
 	p = NULL;
	 len = 0 ;
	 return 0;
 }

//�ͻ��˷�����
 int CSckFactoryImp1::cltSocketSend( unsigned char *buf /*in*/,  int buflen /*in*/)
 {
	 p  = (unsigned char * ) malloc(sizeof(unsigned char)  * buflen);
	 if (p == NULL)
	 {
		 return -1;
	 }
	 memcpy(p, buf, buflen);
	 len = buflen;
	 return 0;
 }

//�ͻ����ձ���
 int CSckFactoryImp1::cltSocketRev( unsigned char *buf /*in*/, int *buflen /*in out*/)
 {
	 if (buf==NULL || buflen==NULL)
	 {
		 return -1;
	 }

	 *buflen  = this->len ;
	 memcpy(buf, this->p, this->len);
	 return 0;
 }

//�ͻ����ͷ���Դ
 int CSckFactoryImp1::cltSocketDestory()
 {
	 if (p != NULL)
	 {
		 free(p);
		 p = NULL;
		 len = 0;
	 }
	 return 0;
 }