
#include <iostream>
using namespace std;
#include "myarray.h"

//��Ŀ��������
//��Ĳ��԰���

//����[]
//void operator[](int i)
//int operator[](int i);
//int& operator[](int i);
void mainArr()
{
	Array  a1(10);

	for (int i=0; i<a1.length(); i++)
	{
		a1.setData(i, i);
		//2 
		a1[i] = i;
		//
		//��������ֵ����ֵ����Ҫ����һ������
		//a1.operator [i]
	}

	cout<<"\n��ӡ����a1: ";
	for (int i=0; i<a1.length(); i++)
	{
		//cout<<a1.getData(i)<<" ";
		//1
		cout<<a1[i]<<endl;
	}
	cout<<endl;

	Array a2 = a1;
	cout<<"\n��ӡ����a2: ";
	for (int i=0; i<a2.length(); i++)
	{
		cout<<a2.getData(i)<<" ";
	}
	cout<<endl;

	//3
	Array a3(5);
	{
		a3 = a1;
		a3 = a2 = a1;

		cout<<"\n��ӡ����a3: ";
		for (int i=0; i<a3.length(); i++)
		{
			cout<<a3[i]<<" ";
		}

		//a3.operator=(a1)
		//Array& operator=(Array &a1)
	}
	

	//����4
	
	if (a3 == a1)
	{
		printf("���\n");
	}
	else
	{
		printf("�����\n");
	}
	//a3.operator==(a1);
	//bool operator==(Array &a1);

	
	if (a3 != a1)
	{
		printf("�����\n");
	}
	else
	{
		printf("���\n");
	}
	//
	//a3.operator!=(a1)
	// bool operator!=(Array &a1);

	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}