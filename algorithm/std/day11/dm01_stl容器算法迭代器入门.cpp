
#include <iostream>
using namespace std;

#include "vector"
#include "algorithm"

//������ װ �����������ͱ���
void main101()
{
	vector<int>  v1; // 1����: �����Ԫ��copy�������� �������캯��
	v1.push_back(-1);
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(3);

	// 1  3  5
	//��
	//2������: �൱��һ��ָ�� ����
	for (vector<int>::iterator it = v1.begin();it != v1.end(); it++)
	{
		cout << *it << " ";
	}

	//3�㷨  �㷨�͵����� �����޷������ 
	int num1 = count(v1.begin(), v1.end(), 3);
	cout << "num1: " << num1 <<endl;

}

class  Teacher
{
public:
	int		age;
	char	name[64];

public:
	void printT()
	{
		cout << "age: " << age << endl;
	}
};

//������ װ Ԫ��
void main102()
{
	Teacher  t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	vector<Teacher>  v1; // 1����: �����Ԫ��copy��������  //����ʵ���� �������� ���㷨����Ч����
	v1.push_back(t1);
	v1.push_back(t2);
	v1.push_back(t3);


	for (vector<Teacher>::iterator it = v1.begin();it != v1.end(); it++)
	{
		cout << it->age << " ";
	}


	//��ש: ����� ���� �� ��ʦ���� ����=33 ����....

}


void main103()
{
	Teacher  t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	Teacher *p1, *p2, *p3 ;
	p1 = &t1;
	p2 = &t2;
	p3 = &t3;

	vector<Teacher *>  v1; // 1����: ��t1 t2 t3�ڴ��׵�ַ ���뵽������
	v1.push_back(p1);
	v1.push_back(p2);
	v1.push_back(p3);

	for (vector<Teacher *>::iterator it = v1.begin();it != v1.end(); it++)
	{
		cout << (*it)->age << endl;
	}

}