#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main11()
{
	int a[] = {1, 2};
	int b[100] = {1, 3};

	int c[200] = {0}; //������� ���Ѿ�ȷ�� ���е�ֵ Ϊ��

	memset(c, 0, sizeof(c)); //��ʾ�� �����ڴ��
	//��һά����  C�涨��
	//c��������Ԫ�صĵ�ַ c+1 ���� 4���ֽ�
	//&c ����������ĵ�ַ //&c+1 ���� 200*4 
}

//��ô�����int a[10]�������������ǣ�int[] 
//���ͱ��ʣ��̶���С�ڴ��ı���
//������������
void main441()
{
	typedef int(MyArrayType)[5]; //������һ����������  ������������
	int i = 0;
	MyArrayType  myArray;  //int myArray[5];

	for (i=0; i<5; i++)
	{
		myArray[i] = i+1;
	}

	for (i=0; i<5; i++)
	{
		printf("%d ", myArray[i]);
	}

	printf("myArray����������Ԫ�صĵ�ַ myArray:%p myArray+1:%p \n", myArray, myArray+1);
	printf("&myArray������������ĵ�ַ &myArray:%p &myArray+1:%p \n", &myArray, &myArray+1);
}


//��������ָ������ķ���1  
//���������� * 
void main13()
{
	char *Myarray[] = {"1111", "33333", "aaaa"}; //ָ�� ����

	//����ָ��  ��һ��ָ�� ��ָ��һ������

	typedef int (MyArrayType)[5]; //������һ����������  ������������
	int i = 0;
	MyArrayType  myArray;  //int myArray[5]; //�����Ͷ������ 

	MyArrayType *pArray; //����һ��ָ����� ���ָ����� ָ��һ������

	{
		int  a;
		int *p = NULL;
		p = &a;

	}
	{
		int myArray2[5]; //�൱��һ��ָ��

		pArray = &myArray2; //�൱��2��ָ��
		for (i=0; i<5; i++)
		{
			(*pArray)[i] = i+1;
		}
	
		for (i=0; i<5; i++)
		{
			printf("%d ", (*pArray)[i]);
		}
	}
}

//��������ָ������ĵڶ��ַ���

void main601a()
{
	//��������һ������ָ������
	typedef int (*PArrayType)[5];
	PArrayType pArray;  //���߱����� ���ҷ���һ��ָ�����

	int c[5];
	int i  = 0;
	pArray = &c;

	for (i=0; i<5; i++)
	{
		(*pArray)[i] = i + 1;
	}

	for (i=0; i<5; i++)
	{
		printf("%d ", (*pArray)[i]);
	}
}

//��������ָ������ĵ����ַ���
//ǰ2�ַ��� ͨ�����Ͷ������ �Ƚ��鷳
void main116()
{
	//int [][5]
	int (*pMyArray)[5];			 //ֱ�Ӷ���һ��ָ������� ����ָ�����
	int c[5];
	int i  = 0;
	pMyArray = &c;

	for (i=0; i<5; i++)
	{
		(*pMyArray)[i] = i + 1;
	}

	for (i=0; i<5; i++)
	{
		printf("%d ", (*pMyArray)[i]);
	}
}

