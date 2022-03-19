#pragma once
#include<iostream>
#include<memory>
using namespace std;


void PrintV(int &t) {
	cout << "lvalue" << endl;
}

void PrintV(int &&t) {
	cout << "rvalue" << endl;
}

template<typename T>
void Test(T &&t) {
	PrintV(t);
	PrintV(std::forward<T>(t));

	PrintV(std::move(t));
}

int mainForward() {
	Test(1); // lvalue rvalue rvalue
	int a = 1;
	Test(a); // lvalue lvalue rvalue
	Test(std::forward<int>(a)); // lvalue rvalue rvalue
	Test(std::forward<int&>(a)); // lvalue lvalue rvalue
	Test(std::forward<int&&>(a)); // lvalue rvalue rvalue
	return 0;
}
/*
Test(1)��1����ֵ��ģ����T &&t����Ϊ�������ã���ֵ1����Test�����б������ֵ���ã����ǵ���PrintV()ʱ��t�������ֵ����Ϊ�������һ��ӵ�����ֵı�����
���Դ�ӡlvalue����PrintV(std::forward<T>(t))ʱ�򣬻��������ת��������ԭ��������ת�������Դ�ӡrvalue��PrintV(std::move(t))�������ʻ��ӡrvalue��

Test(a)��a����ֵ��ģ����T &&����Ϊ�������ã���ֵa����Test�����б������ֵ���ã������д����д�ӡ��

Test(std::forward<T>(a))��ת��Ϊ��ֵ������ֵ��������T��T����ֵ�Ǿ�ת��Ϊ��ֵ��T����ֵ�Ǿ�ת��Ϊ��ֵ��
*/