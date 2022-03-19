#pragma once
#include<iostream>
using namespace std;
// ����1���� inline������ʹ�ã�
inline int functionName(int first, int second, ...);

// ����2������ inline��
int functionName(int first, int second, ...);

// ����
inline int functionName(int first, int second, ...) {/****/ };

// ���ڶ��壬��ʽ����
class A 
{
	int doA() { return 0; }         // ��ʽ����
};

// ���ⶨ�壬��Ҫ��ʽ����
class A 
{
	int doA();
};
inline int A::doA() { return 0; }   // ��Ҫ��ʽ����

//�麯������������������������ִ�п�����ϵͳ
class Base
{
public:
	inline virtual void who()
	{
		cout << "I am Base\n";
	}
	virtual ~Base() {}
};
class Derived : public Base
{
public:
	inline void who()  // ��дinlineʱ��ʽ����
	{
		cout << "I am Derived\n";
	}
};

int mainInlineVirtual()
{
	// �˴����麯�� who()����ͨ���ࣨBase���ľ������b�������õģ������ڼ����ȷ���ˣ������������������ģ��������Ƿ�����ȡ���ڱ�������
	Base b;
	b.who();

	// �˴����麯����ͨ��ָ����õģ����ֶ�̬�ԣ���Ҫ������ʱ�ڼ����ȷ�������Բ���Ϊ������
	Base *ptr = new Derived();
	ptr->who();

	// ��ΪBase��������������virtual ~Base() {}�������� delete ʱ�����ȵ��������ࣨDerived�������������ٵ��û��ࣨBase��������������ֹ�ڴ�й©��
	delete ptr;
	ptr = nullptr;

	system("pause");
	return 0;
}