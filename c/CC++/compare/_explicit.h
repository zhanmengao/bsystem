#pragma once
struct A
{
	A(int) { }
	operator bool() const { return true; }
};

struct B
{
	explicit B(int) {}
	explicit operator bool() const { return true; }
};

void doA(A a) {}

void doB(B b) {}

int mainexplicit()
{
	A a1(1);									// OK��ֱ�ӳ�ʼ��
	A a2 = 1;									// OK�����Ƴ�ʼ��
	A a3{ 1 };										// OK��ֱ���б��ʼ��
	A a4 = { 1 };								// OK�������б��ʼ��
	A a5 = (A)1;								// OK������ static_cast ����ʽת��
	doA(1);												// OK������� int �� A ����ʽת��
	if (a1);												// OK��ʹ��ת������ A::operator bool() �Ĵ� A �� bool ����ʽת��
	bool a6��a1��;									// OK��ʹ��ת������ A::operator bool() �Ĵ� A �� bool ����ʽת��
	bool a7 = a1;										// OK��ʹ��ת������ A::operator bool() �Ĵ� A �� bool ����ʽת��
	bool a8 = static_cast<bool>(a1);					// OK ��static_cast ����ֱ�ӳ�ʼ��

	B b1(1);									// OK��ֱ�ӳ�ʼ��
	//B b2 = 1;									// ���󣺱� explicit ���ι��캯���Ķ��󲻿��Ը��Ƴ�ʼ��
	B b3{ 1 };								// OK��ֱ���б��ʼ��
	//B b4 = { 1 };							// ���󣺱� explicit ���ι��캯���Ķ��󲻿��Ը����б��ʼ��
	B b5 = (B)1;								// OK������ static_cast ����ʽת��
	//doB(1);										// ���󣺱� explicit ���ι��캯���Ķ��󲻿��Դ� int �� B ����ʽת��
	if (b1);								// OK���� explicit ����ת������ B::operator bool() �Ķ�����Դ� B �� bool �İ��ﾳת��
	bool b6(b1);							// OK���� explicit ����ת������ B::operator bool() �Ķ�����Դ� B �� bool �İ��ﾳת��
	//bool b7 = b1;							// ���󣺱� explicit ����ת������ B::operator bool() �Ķ��󲻿�����ʽת��
	bool b8 = static_cast<bool>(b1);						// OK��static_cast ����ֱ�ӳ�ʼ��

	return 0;
}