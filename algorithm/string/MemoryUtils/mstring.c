#include"mstring.h"
#include<math.h>

void* Check(char* dest, const char* src,uint32_t size)
{
	//Ϊ��
	if (dest == NULL || src == NULL)
	{
		return -1;
	}
	else if (fabs(dest - src)< size)
	{
		return -1;
	}
	return 0;
}
uint32_t mstrlen(const char* str)
{
	if (str == NULL)
	{
		return 0;
	}
	uint32_t size = 0;
	while (*str != '\0')
	{
		size++;
		str++;
	}
	return size;
}
char* mstrcpy(char* dest, const char* src)
{
	//�������������
	const char* pSrc = src;
	char* pDest = dest;
	while (*pSrc != '\0')
	{
		*pDest = *pSrc;
		pSrc++;
		pDest++;
	}
	*(pDest) = '\0';
	return dest;
}

void* mmemcpy(void* dst, const void* src, uint32_t count)
{
	void* ret = dst;
	while (count--)
	{
		*(char *)dst = *(char *)src;
		dst = (char *)dst - 1;
		src = (char *)src - 1;
	}
	return ret;
}
/*
���Ŀ�������Դ�������ص��Ļ���memmove() �ܹ���֤Դ���ڱ�����֮ǰ���ص�������ֽڿ�����Ŀ�������У�
���ƺ�Դ��������ݻᱻ���ġ����Ŀ��������Դ����û���ص������ memcpy() ����������ͬ��
*/
void* mmemmove(void* dst, const void* src, uint32_t count)
{
	if (dst <= src || (char *)dst >= ((char *)src + count))
	{
		return mmemcpy(dst, src, count);
	}
	else
	{
		dst = (char *)dst + count - 1;			//�Ƶ�β��
		src = (char *)src + count - 1;

		return mmemcpy(dst, src, count);
	}
}