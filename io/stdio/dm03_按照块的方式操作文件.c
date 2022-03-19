#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//直接把内存数据 写入到 文件中
typedef struct Teacher
{
	char name[64];
	int age ;
}Teacher;

void main_fwrite()
{
	int i = 0;
	FILE *fp = NULL;
	char *fileName = "c:/3.data";
	Teacher tArray[3];
	int  myN = 0;

	for (i=0; i<3; i++)
	{
		sprintf(tArray[i].name, "%d%d%d", i+1, i+1, i+1);
		tArray[i].age = i + 31;
	}

	fp = fopen(fileName, "wb");
	if (fp == NULL)
	{
		printf("建立文件失败\n");
		return ;
	}
	for (i=0; i<3; i++)
	{
		//_Check_return_opt_ _CRTIMP size_t __cdecl 
		//	fwrite(_In_count_x_(_Size*_Count) const void * _Str, _In_ size_t _Size, _In_ size_t _Count, _Inout_ FILE * _File);
		//函数参数
		//_Str : from
		//_Size  大小
		//_Count 写多少次
		//_File : to

		//函数的返回值 
		myN = fwrite( &tArray[i],sizeof(Teacher) , 1, fp);            //往fp写from指向内存的连续size个字节，写一次

		//myN 判断 有没有写满  磁盘

	}

	if (fp != NULL)
	{
		fclose(fp);
	}

}

void main_fread()
{
	int i = 0;
	FILE *fp = NULL;
	char *fileName = "c:/3.data";
	Teacher tArray[3];
	int  myN = 0;



	fp = fopen(fileName, "r+b");
	if (fp == NULL)
	{
		printf("建立文件失败\n");
		return ;
	}
	for (i=0; i<3; i++)
	{
		myN  = fread(&tArray[i], sizeof(Teacher), 1, fp);             //将fp的size字节数据读到to指针，读1次

		//myN 判断 有没有写满  磁盘

	}

	for (i=0; i<3; i++)
	{
		//sprintf(tArray[i].name, "%d%d%d", i+1, i+1, i+1);
		//tArray[i].age = i + 31;
		printf("name:%s, age:%d \n", tArray[i].name, tArray[i].age);
	}

	if (fp != NULL)
	{
		fclose(fp);
	}

}