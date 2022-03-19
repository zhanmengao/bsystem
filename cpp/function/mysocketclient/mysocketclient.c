#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "itcastlog.h"

typedef struct _SCK_HANDLE
{
	char	version[64];
	char	ip[128];
	int		port;
	unsigned char	*p;
	int		plen;
}SCK_HANDLE; //��̬�� �ڲ����������� �������ò��Գ����ϲ�Ӧ��֪����
//�������͵ķ�װ

__declspec(dllexport)
int cltSocketInit(void **handle /*out*/)
{
	int		ret = 0;
	SCK_HANDLE *hdl = NULL;
	ITCAST_LOG(__FILE__, __LINE__, LogLevel[2], ret, "func cltSocketInit() Begin 22222:%d", ret);

	hdl = (SCK_HANDLE *)malloc(sizeof(SCK_HANDLE));
	if (hdl == NULL)
	{
		ret = -1;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketInit() err:%d", ret);
		return ret;
	}
	memset(hdl, 0, sizeof(SCK_HANDLE)); //��ָ����ָ����ڴ�ռ� ��ֵ�� 0��

	strcpy(hdl->ip, "192.168.6.254");
	hdl->port = 8081;
	*handle = hdl;

	ITCAST_LOG(__FILE__, __LINE__, LogLevel[2], ret, "func cltSocketInit() End:%d \n", ret);

	return ret;
}

//�ͻ��˷�����
__declspec(dllexport)
int cltSocketSend(void *handle /*in*/, unsigned char *buf /*in*/,  int buflen /*in*/)
{
	int		ret = 0;
	SCK_HANDLE *hdl = NULL;

	if (handle==NULL || buf==NULL )
	{
		ret = -1;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketSend() err:%d\n  (handle==NULL || buf==NULL ) ", ret);
		return ret;
	}
	
	hdl = (SCK_HANDLE *)handle;

	hdl->p = (unsigned char *)malloc(buflen *sizeof(unsigned char));
	if (hdl->p == NULL)
	{
		ret = -2;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketSend() err: buflen:%d ", buflen);
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketSend() err:%d\n  (unsigned char *)malloc(buflen *sizeof(unsigned char) ", ret);
		return ret;
	}
	memcpy(hdl->p, buf, buflen);
	hdl->plen = buflen;

	return 0;
}

//�ͻ����ձ���
__declspec(dllexport)
int cltSocketRev(void *handle /*in*/, unsigned char *buf /*in*/, int *buflen /*in out*/)
{
	int		ret = 0;
	SCK_HANDLE *hdl = NULL;

	if (handle==NULL || buf==NULL  ||buflen==NULL)
	{
		ret = -1;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketRev() err:%d\n  (handle==NULL || buf==NULL ) ", ret);
		return ret;
	}
	hdl = (SCK_HANDLE *)handle;

	memcpy(buf, hdl->p, hdl->plen);
	*buflen =  hdl->plen;

	return ret;
}

//�ͻ����ͷ���Դ
__declspec(dllexport)
int cltSocketDestory(void *handle/*in*/)
{
	int		ret = 0;
	SCK_HANDLE *hdl = NULL;

	if (handle==NULL )
	{
		ret = -1;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketDestory() err:%d\n  (handle==NULL || buf==NULL ) ", ret);
		return ret;
	}
	hdl = (SCK_HANDLE *)handle;

	if (hdl->p)
	{
		free(hdl->p);
	}
	free(hdl);

	return ret;
}


//-----------------------�ڶ���api����--------------------------------*/
__declspec(dllexport)
int cltSocketInit2(void **handle)
{
	return cltSocketInit(handle);
}

//�ͻ��˷�����
__declspec(dllexport)
int cltSocketSend2(void *handle, unsigned char *buf,  int buflen)
{
	return cltSocketSend(handle, buf, buflen);
}

//�ͻ����ձ���
__declspec(dllexport)
int cltSocketRev2(void *handle, unsigned char **buf, int *buflen)
{
	int		ret = 0;
	SCK_HANDLE *hdl = NULL;
	unsigned char		*tmp = NULL;

	if (handle==NULL || buf==NULL  ||buflen==NULL)
	{
		ret = -1;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketRev2() err:%d\n  (handle==NULL || buf==NULL ) ", ret);
		return ret;
	}
	hdl = (SCK_HANDLE *)handle;

	tmp = (unsigned char *)malloc(hdl->plen);
	if (tmp == NULL)
	{
		ret = -2;
		ITCAST_LOG(__FILE__, __LINE__, LogLevel[4], ret, "func cltSocketRev2() err:%d\n  (malloc err ) ", ret);
		return ret;
	}

	memcpy(tmp, hdl->p, hdl->plen);
	*buflen =  hdl->plen;

	*buf = tmp; //��Ӹ�ֵ
	return ret;
}

__declspec(dllexport)
int cltSocketRev2_Free(unsigned char **buf)
{
	if (buf == NULL)
	{
		return -1;
	}
	if (*buf != NULL)
	{
		free(*buf);
	}
	*buf = NULL; //*ʵ�εĵ�ַ  ȥ��ӵ��޸�ʵ�ε�ֵ  ���³�ʼ��NULL
	return 0;
}

//�ͻ����ͷ���Դ
__declspec(dllexport)
int cltSocketDestory2(void **handle)
{
	SCK_HANDLE *tmp = NULL;
	if (handle==NULL)
	{
		return -1;
	}
	tmp = *handle; 
	if (tmp != NULL)
	{
		if (tmp->p)
		{
			free(tmp->p);
			tmp->p = NULL;
		}
		free(tmp);
	}
	*handle = NULL; //*ʵ�εĵ�ַ  ȥ��ӵ��޸�ʵ�ε�ֵ  ���³�ʼ��NULL

	return 0;
}
