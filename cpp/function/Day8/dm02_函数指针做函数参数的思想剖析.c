#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int myadd(int a, int b)  //�������ʵ����
{
	printf("func add() do...\n");
	return a + b;
}
int myadd2(int a, int b)  //�������ʵ����
{
	printf("func add2() do...\n");
	return a + b;
}
int myadd3(int a, int b)  //�������ʵ����
{
	printf("func add3() do...\n");
	return a + b;
}
int myadd4(int a, int b)  //�������ʵ����
{
	printf("func add4() do...\n");
	return a + b;
}


typedef int (*MyTypeFuncAdd)(int a, int b);     //������һ������ָ������ 

//����ָ�� �� ��������
int MainOp(MyTypeFuncAdd myFuncAdd)
{
	int c = myFuncAdd(5, 6);
	return c;
}

// int (*MyPointerFuncAdd)(int a, int b)
int MainOp2(int (*MyPointerFuncAdd)(int a, int b) )
{
	int c = MyPointerFuncAdd(5, 6); //��ӵ���
	return c;
}

//��ӵ��� 

//����ĵ��� �� ����ı�д���Էֿ�
void main02()
{
	myadd(1, 2); //ֱ�ӵ���

	MyTypeFuncAdd  myFuncAdd = myadd;
	myFuncAdd(3, 4); //��ӵ��� 

	//�ȼ�
	MainOp2(myadd);
	MainOp(myadd);

	//��mainop��� û�з����κα仯������� ...
	MainOp(myadd2);
	MainOp(myadd3);
	MainOp(myadd4);
}