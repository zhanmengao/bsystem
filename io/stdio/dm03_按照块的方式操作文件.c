#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//ֱ�Ӱ��ڴ����� д�뵽 �ļ���
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
		printf("�����ļ�ʧ��\n");
		return ;
	}
	for (i=0; i<3; i++)
	{
		//_Check_return_opt_ _CRTIMP size_t __cdecl 
		//	fwrite(_In_count_x_(_Size*_Count) const void * _Str, _In_ size_t _Size, _In_ size_t _Count, _Inout_ FILE * _File);
		//��������
		//_Str : from
		//_Size  ��С
		//_Count д���ٴ�
		//_File : to

		//�����ķ���ֵ 
		myN = fwrite( &tArray[i],sizeof(Teacher) , 1, fp);            //��fpдfromָ���ڴ������size���ֽڣ�дһ��

		//myN �ж� ��û��д��  ����

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
		printf("�����ļ�ʧ��\n");
		return ;
	}
	for (i=0; i<3; i++)
	{
		myN  = fread(&tArray[i], sizeof(Teacher), 1, fp);             //��fp��size�ֽ����ݶ���toָ�룬��1��

		//myN �ж� ��û��д��  ����

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