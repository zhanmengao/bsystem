#include "stdlib.h"
#include "string.h"
#include "stdio.h"

//����
void main102()
{
	int		i = 0,j = 0;
	int		tmp = 0;
	int a[] = {33,654,4,455,6,33,4};

	printf("����֮ǰ\n");
	for (i=0; i<7; i++)
	{
		printf("%d ", a[i]);
	}
	
	//����

	//���ѭ��	��i=0��ʱ��, ��j��1===N���б仯
	//���ѭ��	��i=1��ʱ��, ��j��2===N���б仯
	//			��i=2��ʱ��, ��j��3===N���б仯
	//����: ����һ������i����,������һ������j���б仯;��һ�� ���ν���

	for(i=0; i<7; i++)  
	{
		for (j=i+1; j<7; j++)  //�ڲ�ѭ��: a[i] �� a[j]�Ƚ�
		{
			if (a[i] > a[j])
			{
				tmp = a[i];
				a[i]= a[j];
				a[j] = tmp;
			}
		}
	}

	printf("����֮��\n");

	for (i=0; i<7; i++)
	{
		printf("%d ", a[i]);
	}
}


//void printArray(int a[7], int num)
//void printArray(int a[], int num)
void printArray(int *a, int num)
{
	int i = 0;
	for (i=0; i<num; i++)
	{
		printf("%d ", a[i]);
	}
	printf("\n");
}

void sortArray(int a[7], int num)                //����int a[7]�˻�Ϊint *a
//void sortArray(int a[], int num)
//void sortArray(int *a, int num)
{
	int i , j , tmp ;
	int		num2 = 0;

	num2 = sizeof(a)/sizeof(a[0]);
	printf("num1:%d,  num2:%d \n",num ,num2);
	//ʵ�ε�a �� �βε�a ���������ͱ��ʲ�һ��
	//�β��е����� ,���������������ָ�봦�� ����C���Ե���ɫ
	for(i=0; i<num; i++) 
	{
		for (j=i+1; j<num; j++)  //�ڲ�ѭ��: a[i] �� a[j]�Ƚ�
		{
			if (a[i] > a[j])
			{
				tmp = a[i];
				a[i]= a[j];
				a[j] = tmp;
			}
		}
	}
}

//�����������������˻�����  �˻�Ϊһ��ָ��, 
//1 ��ȷ��������������ڴ��׵�ַ���������Ч���ȴ��������ú���
//2 //ʵ�ε�a �� �βε�a ���������ͱ��ʲ�һ��
	//�β��е����� ,���������������ָ�봦�� ����C���Ե���ɫ
	//���� ����Ҳ���� 
//3 �β�д�ں�����,��д�ں�������һ����,ֻ�����Ǿ��ж�������Զ���.
void main103()
{
	int		i = 0,j = 0;
	int		tmp = 0;
	int		num = 0;
	int a[] = {33,654,4,455,6,33,4,3333};
	num  = 7;

	num = sizeof(a)/sizeof(a[0]);

	printf("����֮ǰ\n");
	printArray(a, num);

	//����

	//���ѭ��	��i=0��ʱ��, ��j��1===N���б仯
	//���ѭ��	��i=1��ʱ��, ��j��2===N���б仯
	//			��i=2��ʱ��, ��j��3===N���б仯
	//����: ����һ������i����,������һ������j���б仯;��һ�� ���ν���

	sortArray(a, num);

	printf("����֮��\n");
	printArray(a, num);
}