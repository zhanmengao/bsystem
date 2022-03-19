#pragma once

namespace MySpace
{
	class Base
	{
	};
}
using MySpace::Base;				//using����:���������ռ��һ����Ա

class Derived : Base 
{
public:
	using Base::Base;					//�� C++11 �У��������ܹ�������ֱ�ӻ��ඨ��Ĺ��캯����
};
//���� using ���������ڻ����ÿ�����캯����������������һ����֮��Ӧ���β��б���ȫ��ͬ���������๹�캯����
//�����������͹��캯����Derived(parms) : Base(args) { }

using namespace std;					//useָʾ��ʹ��ĳ���ض������ռ����������ֶ��ɼ����������Ǿ�������Ϊ��������κ�ǰ׺�޶�����

//Ӧ�ö�ʹ�� using ����
#include<iostream>
void us1()
{
	int x;
	std::cin >> x;
	std::cout << x << std::endl;
}

void us2()
{
	using std::cin;
	using std::cout;
	using std::endl;
	int x;
	cin >> x;
	cout << x << endl;
}
/*
һ��˵����ʹ�� using �����ʹ�� using �����������ȫ������������ֻ������ָ�������ơ�
�����������ֲ����Ʒ�����ͻ��������������ָʾ��using������������е����ƣ��������ܲ�����Ҫ�����ơ�
�����ֲ����Ʒ�����ͻ����ֲ����ƽ��������ƿռ�汾���������������ᷢ�����档
���⣬���ƿռ�Ŀ�������ζ�����ƿռ�����ƿ��ܷ�ɢ�ڶ���ط�����ʹ������׼ȷ֪���������Щ���ơ�
*/
