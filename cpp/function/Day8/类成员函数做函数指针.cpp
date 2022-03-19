#include <stdio.h>
#include <iostream>

using namespace std;
/*
1. �Ǿ�̬�ĳ�Ա��������ָ���﷨��ͬC���Բ�ࣩ��
void (*ptrStaticFun)() = &ClassName::staticFun;

2. ��Ա��������ָ���﷨��
void (ClassName::*ptrNonStaticFun)() = &ClassName::nonStaticFun;
ע��������зǾ�̬��Ա������ʱ��ʹ�õ��� ����::�������������� ʵ����::��������
*/
class MyClass 
{
public:
	static int FunA(int a, int b)
	{
		cout << "call FunA" << endl;
		return a + b;
	}

	void FunB() 
	{
		cout << "call FunB" << endl;
	}

	void FunC() 
	{
		cout << "call FunC" << endl;
	}

	int pFun1(int(*p)(int, int), int a, int b) 
	{
		return (*p)(a, b);
	}

	void pFun2(void (MyClass::*nonstatic)()) 
	{
		(this->*nonstatic)();
	}
};

int mainO1()
{
	MyClass* obj = new MyClass;
	// ��̬����ָ���ʹ��
	int(*pFunA)(int, int) = &MyClass::FunA;
	cout << pFunA(1, 2) << endl;

	// ��Ա����ָ���ʹ��
	void (MyClass::*pFunB)() = &MyClass::FunB;
	(obj->*pFunB)();

	// ͨ�� pFun1 ֻ�ܵ��þ�̬����
	obj->pFun1(&MyClass::FunA, 1, 2);

	// ͨ�� pFun2 ���ǵ��ó�Ա����
	obj->pFun2(&MyClass::FunB);
	obj->pFun2(&MyClass::FunC);

	delete obj;
	return 0;
}