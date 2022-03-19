
#include <iostream>
using namespace std;
#include "stack"

//ջģ�� 
//ջ���㷨 �� �������͵ķ���
void main5011()
{
	stack<int> s;

	//��ջ
	for (int i=0; i<10; i++)
	{
		s.push(i+1);
	}
	cout << "ջ�Ĵ�С" << s.size() << endl;

	//��ջ
	while ( !s.empty())
	{
		int tmp = s.top(); //��ȡջ��Ԫ��
		cout <<  tmp << " ";
		s.pop(); //����ջ��Ԫ�� 
	}
}

//teacher���
class Teacher
{
public:
	int		age;
	char	name[32];
public:
	void printT()
	{
		cout << "age:" << age << endl;
	}

};

void main5052()
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	stack<Teacher> s;
	s.push(t1);
	s.push(t2);
	s.push(t3);

	while ( !s.empty())
	{
		Teacher tmp = s.top();
		tmp.printT();
		s.pop();
	}
}

void main503()
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	stack<Teacher *> s;
	s.push(&t1);
	s.push(&t2);
	s.push(&t3);

	while ( !s.empty())
	{
		Teacher *p = s.top();
		p->printT();
		s.pop();
	}
}