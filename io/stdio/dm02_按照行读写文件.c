#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main21_fputs()
{
	int		i = 0;
	FILE *fp = NULL;
	//char *filename = "c:\\1.txt";
	char *filename2 = "c:/22.txt";  // ͳһ����45�� 
	char a[27]= "abcdefghijklmn";
	//fp = fopen(filename2, "r+");   //��д�ķ�ʽ ���ļ� ����ļ������� �򱨴�
	fp = fopen(filename2, "w+");   //��д�ķ�ʽ ���ļ� ����ļ������� �򱨴�
	if (fp == NULL)
	{
		printf("func fopen() err:%d \n");
		return;
	}
	printf("�򿪳ɹ�\n");

	/*
	for (i=0; i<strlen(a); i++)
	{
		fputc(a[i], fp);
	}
	*/

	fputs(a, fp);            //���ַ���aд�뵽fp���ļ�ָ��

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
	char *filename2 = "c:/22.txt";  // ͳһ����45�� 
	//char a[27]= "abcdefghijklmn";
	char buf[1024];
	fp = fopen(filename2, "r+"); //����д��ʽ
	if (fp == NULL)
	{
		printf("func fopen() err:%d \n");
		return;
	}
	printf("�򿪳ɹ�\n");

	//1 //C ������ �� һ��һ�е�copy���� ��bufָ����ָ���ڴ�ռ��� ���ұ��C�����ַ���
	//2 ��\nҲcopy�����ǵ�buf��
	//3 �ڴ��� (���ڴ��׵�ַ + �ڴ�ĳ���)
	while (!feof(fp))
	{
		char *p = fgets(buf, 1024, fp);  //C ������ �� һ��һ�е�copy���� ��bufָ����ָ���ڴ�ռ��� ���ұ��C�����ַ���
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