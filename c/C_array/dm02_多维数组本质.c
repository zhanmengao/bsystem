#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void printArray01(int a[3][5]) //4k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}
}

void printArray02(int a[][5]) //5k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}
}

void printArray03( int (*b)[5]) //6k
{
	int i, j, tmp = 0;
	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", b[i][j]);
		}
	}
}

//��ά�������������� �˻����̡�������������ʵ���ڡ�����
//Ϊʲô 
void main()
{
	int a[3][5], i=0, j=0;
	int tmp = 1;

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			a[i][j] = tmp++;
		}
	}

	printf("----------------\n");
	printArray03(a);

	for (i=0; i<3; i++)
	{
		for (j=0; j<5; j++)
		{
			printf("%d ", a[i][j]);
		}
	}

	// a ��ά������ ���� ��
	printf("a %d , a+1:%d ", a, a+1);  //a+1�Ĳ��� ��20���ֽ� 5*4
	printf("&a %d , &a+1:%d ", &a, &a+1);

	{
		//����һ��ָ�������ָ����� 
		int (*pArray)[5] ;//���߱�����  ���� 4���ֽڵ��ڴ� 32bitƽ̨��
		pArray = a;

		printf("\n");
		for (i=0; i<3; i++)
		{
			for (j=0; j<5; j++)
			{
				printf("%d ", pArray[i][j]);
			}
		}
	}
	printf("\n");

	//��ά�������ı���  ����ָ�� ������ һά�ĳ���

	//
	// (a+i) ������������i�еĵ�ַ  ����ָ��
	// *(a+i) ���� 1��ָ��  ��i����Ԫ�صĵ�ַ
	// *(a+i) + j  ===> &  a[i][j]

	//*( *(a+i) + j) ===>a[i][j]Ԫ�ص�ֵ


	//a[i][j] <=== >*( *(a+i) + j)

	//a[i] ===> a[0+i] ==> *(a+i);

	//a[i][j] === a[0+i][j]  ==>  *(a+i)[j]  ===>  *(a+i)[0 + j] ==>  *( *(a+i)  + j) 
	const char* str[] = { "aa","bbb","cccc" };
	const char** pstr;
	printf("str %d  pstr = %d\n", sizeof(str),sizeof(pstr));
}