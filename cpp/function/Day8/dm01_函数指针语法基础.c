
#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//����ָ�� �﷨ ����
//����һ����������   
//int a[10];//

//����һ��ָ���������� 

//����һ��ָ�� �������͵�ָ�� �������ָ��

void main101()
{
	int a[10]; //a�������������Ԫ�صĵ�ַ  &a������������ĵ�ַ  a+1 4  &a+1���� 40 .

	{
		//����һ����������         --->�������int[10]���͵ı���
		typedef int (myTypeArray)[10]; //
		myTypeArray myArray;
		myArray[0] = 10;
		printf("%d \n", myArray[0]);
	}

	{
		//����һ��ָ����������    ----->�������ָ��(int[10])����ָ��ı���
		typedef int (*PTypeArray)[10];  //int *p 

		PTypeArray myPArray; //sizeof(int) *10
		myPArray = &a;
		//int b = 10;
		//int *p = NULL;
		//p = &b;
		(*myPArray)[0] = 20;

		printf("a[0]: %d \n", a[0]);
	
	}

	{
		//����һ��ָ�� �������͵�ָ�� �������ָ��   ----->�������ָ�����

		int (*MyPointer)[10]; //���� ����C������ ���ҷ����ڴ�
		MyPointer = &a;
		(*MyPointer)[0] = 40;
		printf("a[0]: %d \n", a[0]);
	}

}



//����ָ���﷨����
//1 ��ζ���һ����������
//2 ��ζ���һ������ָ������
//3 ��ζ���һ�� ����ָ��  (ָ��һ����������ڵ�ַ)

int add(int a, int b)
{
	printf("func add ....\n");
	return a +b;
}

void main102()
{
	add(1, 2); //ֱ�ӵ��õ��� //���������Ǻ�������ڵ�ַ 

	//����һ����������
	{
		typedef int (MyFuncType)(int a, int b); //������һ������
		MyFuncType *myPointerFunc = NULL; //������һ��ָ��, ָ��ĳһ����ĺ���..

		myPointerFunc  = &add;  //ϸ��
		myPointerFunc(3, 4); //��ӵ���

		myPointerFunc  = add;  //ϸ�� //C ���� ������ʷ�汾��ԭ��
		myPointerFunc(3, 4); //��ӵ���
	}

	//����һ������ָ������
	{
		typedef int (*MyPointerFuncType)(int a, int b); //int * a = NULL;
		MyPointerFuncType myPonterFunc; //����һ��ָ��
		myPonterFunc = add;
		myPonterFunc(5, 6);
	}

	//����ָ�� 
	{
		int (*MyPonterFunc)(int a, int b); //������һ������
		MyPonterFunc = add;
		MyPonterFunc(7, 8);
	}
}

