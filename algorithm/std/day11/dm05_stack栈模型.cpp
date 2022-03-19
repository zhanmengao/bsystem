
#include <iostream>
using namespace std;
#include "stack"

//栈模型 
//栈的算法 和 数据类型的分离
void main5011()
{
	stack<int> s;

	//入栈
	for (int i=0; i<10; i++)
	{
		s.push(i+1);
	}
	cout << "栈的大小" << s.size() << endl;

	//出栈
	while ( !s.empty())
	{
		int tmp = s.top(); //获取栈顶元素
		cout <<  tmp << " ";
		s.pop(); //弹出栈顶元素 
	}
}

//teacher结点
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