#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

//演示：指针数组的用法
//演示：找错误 注意return

//求关键字在表中的位置
//一个入口 多个出口
int searcheKeyTable(const char* table[], const int size, const char* key, int *pos)
{
	int rv = 0;
	int i = 0;
	int inum = 0;
	if (table==NULL || key==NULL || pos==NULL)
	{
		rv = -1;
		printf("func searcheKeyTable:%d", rv);
		return rv;
	}

	//间接的证明  数组做函数参数的退回  
	inum = (sizeof(table)/sizeof(*table));

	for(i=0; i<size; i++)
	{
		if( strcmp(key, table[i]) == 0 )
		{
			*pos = i;
			//break;
			return rv;	
		}
	}

	//没有找到返回-1
	if (i == size)
	{
		*pos = -1;
	}
	return rv;
}

#define DIM(a) (sizeof(a)/sizeof(*a))

int main411()
{
	int inum = 0;
	int pos = 0;
	int a[10];
	int i = 0;
	//指针数组
	char*   c_keyword[] = {
		"while", 
		"case",
		"static",
		"do"
	};

	searcheKeyTable( c_keyword, DIM(c_keyword),"do", &pos);
	// ===> 带参数的宏
	//searcheKeyTable( c_keyword, (sizeof(c_keyword)/sizeof(*c_keyword)),"do", &pos);
	//searcheKeyTable( c_keyword, inum,"do", &pos);
	printf("pos:%d\n", pos);
	//searcheKeyTable(NULL, DIM(c_keyword),"do", &pos);
	//printf("pos:%d\n", pos);
	searcheKeyTable( c_keyword, DIM(c_keyword), "static", &pos);
	printf("pos:%d\n", pos);
}

//main函数是操作系统调用的函数
//在程序执行的时候可以向main函数传递参数

/*
argc 命令行参数
argv 命令行参数数组
env  函数变量数组

int main();
int main(int argc);
int main(int argc, char *argv[])
*/


int main477(int argc, char* argv[], char**env)
{
	int i = 0;
	//main02_1();

	printf("******************* Begin argv *******************\n");
	for(i=0; i<argc; i++)
	{
		printf("%s\n", argv[i]);
	}

	// 	for(i=0; argv[i]!=NULL; i++)
	// 	{
	// 		printf("%s\n", argv[i]);
	// 	}
	printf("******************* End argv *******************\n");

	printf("\n");
	printf("\n");
	printf("\n");

	printf("******************* Begin env *******************\n");

	for(i=0; env[i]!=NULL; i++)  //
	{
		printf("%s\n", env[i]);
	}

	printf("******************* End env*******************\n");

	getchar();
}

void main4041()
{
	int inum = 0;
	int pos = 0;
	int a[10];
	int i = 0;
	//指针数组  自我结束能力
	char*   c_keyword[] = {
		"while", 
		"case",
		"static",
		"do",
		'\0'
	};  

	char*   c_keyword2[] = {
		"while", 
		"case",
		"static",
		"do",
		0
	}; 


	char*   c_keyword3[] = {
		"while", 
		"case",
		"static",
		"do",
		NULL
	}; 

	for (i=0; c_keyword[i] != NULL; i++)
	{
		printf("%s\n", c_keyword[i]);
	}
	printf("\n....\n");
	for (i=0; c_keyword2[i] != NULL; i++)
	{
		printf("%s\n", c_keyword2[i]);
	}
	printf("\n....\n");
	for (i=0; c_keyword3[i] != NULL; i++)
	{
		printf("%s\n", c_keyword3[i]);
	}
}






















int main45()
{
	int inum = 0;
	int pos = 0;
	int a[10];
	int i = 0;
	//指针数组
	char*   c_keyword[] = {
		"while", 
		"case",
		"static",
		"do",
		'\0'
	};
	// NULL   0  '\0'


	for(i=0; c_keyword[i]!=NULL; i++)
	{
		printf("%s\n", c_keyword[i]);
	}
	system("pause");
}

