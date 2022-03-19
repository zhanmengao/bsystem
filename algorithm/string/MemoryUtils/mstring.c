#include"mstring.h"
#include<math.h>

void* Check(char* dest, const char* src,uint32_t size)
{
	//为空
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
	//特殊条件按检测
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
如果目标区域和源区域有重叠的话，memmove() 能够保证源串在被覆盖之前将重叠区域的字节拷贝到目标区域中，
复制后源区域的内容会被更改。如果目标区域与源区域没有重叠，则和 memcpy() 函数功能相同。
*/
void* mmemmove(void* dst, const void* src, uint32_t count)
{
	if (dst <= src || (char *)dst >= ((char *)src + count))
	{
		return mmemcpy(dst, src, count);
	}
	else
	{
		dst = (char *)dst + count - 1;			//移到尾部
		src = (char *)src + count - 1;

		return mmemcpy(dst, src, count);
	}
}