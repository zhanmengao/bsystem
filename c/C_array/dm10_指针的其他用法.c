#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int  getContentLen01(const char *filename, char **buf, int *len)
{
	char *tmp = NULL;
	tmp = (char *)malloc(strlen(filename)*sizeof(char));
	if (tmp == NULL)
	{
		return -1;
	}
	strcpy(tmp, "aaaaaaaaaaaaaaaaaaaaaaaaaa");
	*len = 10;
	*buf = tmp; //��Ӹ�ֵ
	return 0;
}

int  getContentLen02(const char *filename, char *buf, int *len)
{
	char *tmp = NULL;
	tmp = (char *)malloc(strlen(filename) *sizeof(char));
	if (buf == NULL)
	{
		 *len = 10; //��һ�ε����󳤶�
	}
	else
	{
		strncpy(tmp, "aaaaaaaaaaaaaaaaaaaaaaaaaa", 10);
		*len = 10;
	}
	return 0;
}

void main21222()
{
	//char buf[1024*20];
	const char *filename = "c:/1.txt";
	char *p = NULL;
	int len = 0;

	getContentLen01(filename, &p, &len);

	if (p != NULL)
	{
		free(p); 
		p = NULL;
	}


	//��һ�ε����󳤶�
	 getContentLen02(filename, NULL, &len);
	


	p = (char *)malloc(len);
	if (p == NULL)
	{
		return ;
	}

	//�ڶ��ε���������
	getContentLen02(filename, p, &len);

	if (p != NULL)
	{
		free(p); 
		p = NULL;
	}
}