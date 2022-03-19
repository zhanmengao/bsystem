#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "socketclientdll.h"
#include "memwatch.h"

int  main01()
{
	int		ret = 0;
	void	*handle = NULL;

	char buf[128]; /*in*/
	int buflen = 3;/*in*/

	char outbuf[128]; /*in*/
	int outbuflen = 3;/*in*/
	strcpy(buf, "dsssssssssdswdfafd");

	ret = cltSocketInit(&handle /*out*/); 
	if (ret != 0)
	{
		printf("func cltSocketInit() err:%d \n", ret);
		return ret;
	}

	//客户端发报文
	ret =  cltSocketSend(handle /*in*/, buf /*in*/, buflen /*in*/);
	if (ret != 0)
	{
		printf("func cltSocketSend() err:%d \n", ret);
		return ret;
	}

	//客户端收报文
	ret =  cltSocketRev(handle /*in*/, outbuf /*in*/, &outbuflen /*in out*/);
	if (ret != 0)
	{
		printf("func cltSocketRev() err:%d \n", ret);
		return ret;
	}
	

	//客户端释放资源
	 cltSocketDestory(handle/*in*/);

	system("pause");
	return ret ;
}


int main()
{
	int		ret = 0;
	void	*handle = NULL;

	char buf[128]; /*in*/
	int buflen = 3;/*in*/

	//char outbuf[128]; /*in*/ outbuf //常量指针
	//int outbuflen = 3;/*in*/

	char	*pout = NULL;
	int		poutlen = 0;
	strcpy(buf, "dsssssssssdswdfafd");

	ret = cltSocketInit2(&handle); 

	//buflen = -133332;


	//客户端发报文
	ret = cltSocketSend2(handle, buf,  buflen);
	if (ret != 0)
	{
		return ret;
	}

	//客户端收报文
	ret = cltSocketRev2(handle, &pout, &poutlen);  //在动态库里面分配内存了...
	if (ret != 0)
	{
		return ret;
	}
	/*
	if (pout != NULL)
	{
		free(pout);
	}
	*/
	cltSocketRev2_Free(&pout); //避免野指针  把outbuf所指向的内存释放,同时把outbuf变量赋值NULL

	//ret = cltSocketRev2_Free(&pout);
	//客户端释放资源

	cltSocketDestory2(&handle);
	system("pause");
	return ;
}



