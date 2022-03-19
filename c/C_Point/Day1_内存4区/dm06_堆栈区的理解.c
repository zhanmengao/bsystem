

#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//分配堆内存
char *getMem161(int num)
{
	char *p1 = NULL;
	p1 = (char *)malloc(sizeof(char) * num);
	if (p1 == NULL)
	{
		return NULL;
	}
	return p1;
}

//栈
//注意 return不是把内存块 64个字节,给return出来,而是把内存块的首地址(内存的标号0xaa11) ,返回给 tmp

// 理解指针的关键,是内存. 没有内存哪里的指针 

//错误的行为：返回栈内存，该内存在函数退出后已经被析构
char *getMem162()
{
	char buf[64]; //临时变量 栈区存放
	strcpy(buf, "123456789");
	return buf;
}

void main601()
{
	char *tmp = NULL;
	tmp = getMem161(10);
	if (tmp == NULL)
	{
		return ;
	}
	strcpy(tmp, "111222"); //向tmp做指向的内存空间中copy数据
	printf("getMem= %s \n", tmp);

	tmp = getMem162();
	printf("getMem2= %s \n", tmp);
	tmp = 0xaa11;

}