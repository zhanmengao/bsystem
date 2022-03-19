#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct  _AdvTeacher
{
	char name[64];
	char *tile;
	char **pStuArray;
	int age;
}Teacher7;

int printTArray(Teacher7*tArray, int num)
{
	int i = 0, j = 0;
	for (i = 0; i < num; i++)
	{
		printf("\n��ʦ��ϢΪ��");
		printf("%d %s %s \n", tArray[i].age, tArray[i].name, tArray[i].tile);
		printf("\nѧ����ϢΪ��");
		for (j = 0; j < 3; j++)
		{
			printf("%s", tArray[i].pStuArray[j]);
		}
	}
	return 0;
}

int sortTArray(Teacher7 *tArray, int num)
{
	int i = 0, j = 0;
	Teacher7 tmp;
	for (i = 0; i < num; i++)
	{
		for (j = i + 1; j< num; j++)
		{
			if (strcmp(tArray[i].name,tArray[j].name)>0)
			{
				tmp = tArray[i];
				tArray[i] = tArray[j];
				tArray[j] = tmp;
			}
		}
	}
	return 0;
}

Teacher7 *creatTArray(int num)
{
	int i = 0, j = 0;
	Teacher7*tArray = NULL;
	tArray = (Teacher7*)malloc(num*sizeof(Teacher7));
	if (tArray == NULL)
	{
		return NULL;
	}
	for (i = 0; i < num; i++)
	{
		tArray[i].tile = (char *)malloc(100);
	}
	for (i = 0; i < num; i++)
	{
		char**ptmp = (char**)malloc((3 + 1)*sizeof(char*));
		for (j = 0; j < 3; j++)
		{
			ptmp[j] = (char*)malloc(120);
		}
		tArray[i].pStuArray = ptmp;
	}
	return tArray;
}

void main3()
{
	int i = 0, j = 0;
	int ret = 0;
	Teacher7 *pArray = NULL;
	pArray = creatTArray(3);
	if (pArray == NULL)
	{
		return;
	}
	for (i = 0; i < 3;i++)
	{
		printf("�����%d����ʦ�����䣺", i + 1);
		scanf("%d", &pArray[i].age);
		printf("�����%d����ʦ������:", i + 1);
		scanf("%s", pArray[i].name);
		printf("�����%d����ʦ��ְ��:", i + 1);
		scanf("%s", pArray[i].tile);

		printf("����ѧ����Ϣ\n");
		for (j = 0; j < 3; j++)
		{
			printf("�����%d��ѧ�����֣�", j + 1);
			scanf("%s", pArray[i].pStuArray[j]);
		}
	}

	printf("����֮ǰ\n");
	printTArray(pArray, 3);
	sortTArray(pArray, 3);
	printf("����֮��\n");
	printTArray(pArray, 3);

}
