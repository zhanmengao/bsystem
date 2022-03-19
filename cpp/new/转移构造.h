#pragma once
#include <vector>
#include <string>
#include <iostream>
using namespace std;

struct Person
{
	string name;
	int age;
	//��ʼ���캯��
	Person(string p_name, int p_age) : name(std::move(p_name)), age(p_age)
	{
		cout << "I have been constructed" << endl;
	}
	//�������캯��
	Person(const Person& other) : name(std::move(other.name)), age(other.age)
	{
		cout << "I have been copy constructed" << endl;
	}
	//ת�ƹ��캯��
	Person(Person&& other) : name(std::move(other.name)), age(other.age)
	{
		cout << "I have been moved" << endl;
	}
};

int main()
{
	vector<Person> e;
	cout << "emplace_back:" << endl;
	e.emplace_back("Jane", 23); //���ù��������

	vector<Person> p;
	cout << "push_back:" << endl;
	p.push_back(Person("Mike", 36));
	return 0;
}
//��������
//emplace_back:
//I have been constructed
//push_back:
//I have been constructed
//I am being moved.