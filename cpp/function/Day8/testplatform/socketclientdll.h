
//written by  wangbaoming1999@163.com
//
/*
下面定义了一套socket客户端发送报文接受报文的api接口
请写出这套接口api的调用方法
*/

/*
//这是一套动态库，实现了客户端和服务器端报文的发送。。
是客户端api函数
*/

#ifndef _INC_Demo01_H
#define _INC_Demo01_H

#ifdef  __cplusplus
extern "C" {
#endif
	

	//定义一套协议
	//实现 动态库 加密解密业务模型抽象
	typedef int (*EncData)(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen);
	typedef int (*DecData)(unsigned char *inData,int inDataLen,unsigned char *outData,int *outDataLen,void *Ref, int RefLen);

	//------------------第一套api接口---Begin--------------------------------//
	//客户端初始化 获取handle上下
	int cltSocketInit(void **handle /*out*/); 
	
	//客户端发报文
	int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/,  int buflen /*in*/);
		
	int cltSocket_EncAndSend(void *handle,  EncData encDataCallBack, unsigned char *buf,  int buflen);
	
	//客户端收报文
	int cltSocketRev(void *handle /*in*/, unsigned char *buf /*in*/, int *buflen /*in out*/);
	
	//客户端释放资源
	int cltSocketDestory(void *handle/*in*/);
	//------------------第一套api接口---End-----------------------------------//
	
	
	//------------------第二套api接口---Begin--------------------------------//
	int cltSocketInit2(void **handle); 

	//客户端发报文
	int cltSocketSend2(void *handle, unsigned char *buf,  int buflen);
	//客户端收报文
	int cltSocketRev2(void *handle, unsigned char **buf, int *buflen);
	int cltSocketRev2_Free(unsigned char **buf);
	//客户端释放资源

	int cltSocketDestory2(void **handle);
	//------------------第二套api接口---End--------------------------------//

	//实现了 把上层应用加密接口入口地址 赛入到动态库 里面
	int cltSocketSetEncFunc(void *handle, EncData encDataFunc,void  *enc_ref, int enc_refLen);


#ifdef  __cplusplus
}
#endif

#endif  /* _INC_Demo01_H */


