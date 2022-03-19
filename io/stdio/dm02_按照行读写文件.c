#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main21_fputs()
{
	int		i = 0;
	FILE *fp = NULL;
	//char *filename = "c:\\1.txt";
	char *filename2 = "c:/22.txt";  // 统一的用45度 
	char a[27]= "abcdefghijklmn";
	//fp = fopen(filename2, "r+");   //读写的方式 打开文件 如果文件不存在 则报错
	fp = fopen(filename2, "w+");   //读写的方式 打开文件 如果文件不存在 则报错
	if (fp == NULL)
	{
		printf("func fopen() err:%d \n");
		return;
	}
	printf("打开成功\n");

	/*
	for (i=0; i<strlen(a); i++)
	{
		fputc(a[i], fp);
	}
	*/

	fputs(a, fp);            //把字符串a写入到fp的文件指针

	fclose(fp);


	return ;
}

/*
char * myfgets(char *myp, int max, FILE *fp)
{
	strcpy(myp, "aaaaaa");
	return myp;
}
*/
void main22_fgets()
{
	int		i = 0;
	FILE *fp = NULL;
	//char *filename = "c:\\1.txt";
	char *filename2 = "c:/22.txt";  // 统一的用45度 
	//char a[27]= "abcdefghijklmn";
	char buf[1024];
	fp = fopen(filename2, "r+"); //读和写方式
	if (fp == NULL)
	{
		printf("func fopen() err:%d \n");
		return;
	}
	printf("打开成功\n");

	//1 //C 函数库 会 一行一行的copy数据 到buf指针所指的内存空间中 并且变成C风格的字符串
	//2 把\n也copy到我们的buf中
	//3 内存打包 (把内存首地址 + 内存的长度)
	while (!feof(fp))
	{
		char *p = fgets(buf, 1024, fp);  //C 函数库 会 一行一行的copy数据 到buf指针所指的内存空间中 并且变成C风格的字符串
		if (p == NULL)
		{
			goto End;
		}	
		printf("%s", buf);
	}
End:
	if (fp != NULL)
	{
		fclose(fp);
	}

	return ;
}