
//written by  wangbaoming1999@163.com
//20140323 23:10
/*
下面定义了一套socket客户端发送报文接受报文的api接口
请写出这套接口api的调用方法
*/

#ifndef _INC_Demo01_H
#define _INC_Demo01_H

#ifdef  __cplusplus
extern "C" {
#endif
	
	//------------------第一套api接口---Begin--------------------------------//
	//客户端初始化 获取handle上下
	int cltSocketInit(void **handle /*out*/); 
	
	//客户端发报文
	int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/,  int buflen /*in*/);
	
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
	
#ifdef  __cplusplus
}
#endif

#endif  /* _INC_Demo01_H */


