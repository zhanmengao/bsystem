#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

class Async;
using AsyncCallBack = int(Async::*)(int a, int b);     //������һ������ָ������ 
class Async
{
public:
	Async()
	{
		//MainOp(myadd2);        //������������ȱ�ٲ����б���ʹ��&Async:myadd2����ָ���Ա��ָ�� 
		//MainOp(Async::myadd3);
		MainOp(&Async::myadd4);
	}
public:
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
//����ָ�� �� ��������
int MainOp(AsyncCallBack myFuncAdd)
{
	int c = (this->*myFuncAdd)(5, 6);
	return c;
}
protected:
private:
};

//��ӵ��� 

//����ĵ��� �� ����ı�д���Էֿ�
void main03()
{
	Async aaa;
}