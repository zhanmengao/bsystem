#pragma once
#ifdef __linux_
#include <bits/stdc++.h>
#endif
#include<iostream>
using namespace std;

class Base
{
public:
	Base() :b(1) {}
	virtual void fun() {};
	int b;
};

class Son : public Base
{
public:
	Son() :d(2) {}
	int d;
};

int main()
{
	int n = 97;

	//reinterpret_cast
	int *p = &n;
	//��������Ч����ͬ
	char *c = reinterpret_cast<char*> (p);
	char *c2 = (char*)(p);
	cout << "reinterpret_cast�����" << *c2 << endl;

	//const_cast
	const int *p2 = &n;
	int *p3 = const_cast<int*>(p2);
	*p3 = 100;
	cout << "const_cast�����" << *p3 << endl;

	Base* b1 = new Son;
	Base* b2 = new Base;

	//static_cast
	Son* s1 = static_cast<Son*>(b1); //ͬ����ת��
	Son* s2 = static_cast<Son*>(b2); //����ת��������ȫ
	cout << "static_cast�����" << endl;
	cout << s1->d << endl;
	cout << s2->d << endl; //����ת����ԭ�ȸ�����û��d��Ա���������ֵ

	 //dynamic_cast
	Son* s3 = dynamic_cast<Son*>(b1); //ͬ����ת��
	Son* s4 = dynamic_cast<Son*>(b2); //����ת������ȫ
	cout << "dynamic_cast�����" << endl;
	cout << s3->d << endl;
	if (s4 == nullptr)
		cout << "s4ָ��Ϊnullptr" << endl;
	else
		cout << s4->d << endl;


	return 0;
}
//������
//reinterpret_cast�����a
//const_cast�����100
//static_cast�����
//2
//-33686019
//dynamic_cast�����
//2
//s4ָ��Ϊnullptr