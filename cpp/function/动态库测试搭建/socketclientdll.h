
//written by  wangbaoming1999@163.com
//20140323 23:10
/*
���涨����һ��socket�ͻ��˷��ͱ��Ľ��ܱ��ĵ�api�ӿ�
��д�����׽ӿ�api�ĵ��÷���
*/

#ifndef _INC_Demo01_H
#define _INC_Demo01_H

#ifdef  __cplusplus
extern "C" {
#endif
	
	//------------------��һ��api�ӿ�---Begin--------------------------------//
	//�ͻ��˳�ʼ�� ��ȡhandle����
	int cltSocketInit(void **handle /*out*/); 
	
	//�ͻ��˷�����
	int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/,  int buflen /*in*/);
	
	//�ͻ����ձ���
	int cltSocketRev(void *handle /*in*/, unsigned char *buf /*in*/, int *buflen /*in out*/);
	
	//�ͻ����ͷ���Դ
	int cltSocketDestory(void *handle/*in*/);
	//------------------��һ��api�ӿ�---End-----------------------------------//
	
	
	//------------------�ڶ���api�ӿ�---Begin--------------------------------//
	int cltSocketInit2(void **handle); 

	//�ͻ��˷�����
	int cltSocketSend2(void *handle, unsigned char *buf,  int buflen);
	//�ͻ����ձ���
	int cltSocketRev2(void *handle, unsigned char **buf, int *buflen);
	int cltSocketRev2_Free(unsigned char **buf);
	//�ͻ����ͷ���Դ

	int cltSocketDestory2(void **handle);
	//------------------�ڶ���api�ӿ�---End--------------------------------//
	
#ifdef  __cplusplus
}
#endif

#endif  /* _INC_Demo01_H */


