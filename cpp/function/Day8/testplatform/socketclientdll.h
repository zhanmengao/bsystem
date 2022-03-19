
//written by  wangbaoming1999@163.com
//
/*
���涨����һ��socket�ͻ��˷��ͱ��Ľ��ܱ��ĵ�api�ӿ�
��д�����׽ӿ�api�ĵ��÷���
*/

/*
//����һ�׶�̬�⣬ʵ���˿ͻ��˺ͷ������˱��ĵķ��͡���
�ǿͻ���api����
*/

#ifndef _INC_Demo01_H
#define _INC_Demo01_H

#ifdef  __cplusplus
extern "C" {
#endif
	

	//����һ��Э��
	//ʵ�� ��̬�� ���ܽ���ҵ��ģ�ͳ���
	typedef int (*EncData)(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen);
	typedef int (*DecData)(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen);

	//------------------��һ��api�ӿ�---Begin--------------------------------//
	//�ͻ��˳�ʼ�� ��ȡhandle����
	int cltSocketInit(void **handle /*out*/); 
	
	//�ͻ��˷�����
	int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/,  int buflen /*in*/);
		
	int cltSocket_EncAndSend(void *handle,  EncData encDataCallBack, unsigned char *buf,  int buflen);
	
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

	//ʵ���� ���ϲ�Ӧ�ü��ܽӿ���ڵ�ַ ���뵽��̬�� ����
	int cltSocketSetEncFunc(void *handle, EncData encDataFunc,void  *enc_ref, int enc_refLen);


#ifdef  __cplusplus
}
#endif

#endif  /* _INC_Demo01_H */


