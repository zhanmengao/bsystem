
#include <iostream>
using namespace std;

class Test4
{
public:
	Test4()  //无参数构造函数
	{
		m_a = 0;
		m_b = 0;
		cout << "无参数构造函数" << endl;
	}

	Test4(int a)
	{
		m_a = a;
		m_b = 0;
	}

	Test4(int a, int b) //有参数构造函数   //3种方法
	{
		m_a = a;
		m_b = b;
		cout << "有参数构造函数" << endl;
	}

	//赋值构造函数 (copy构造函数) //
	/*explicit*/ Test4(const Test4& obj)
	{
		cout << "我也是构造函数 " << endl;
		m_b = obj.m_b + 100;
		m_a = obj.m_a + 100;
	}

public:
	void printT()
	{
		cout << "普通成员函数" << endl;
		cout << "m_a" << m_a << " m_a" << m_b << endl;
	}
private:
	int m_a;
	int m_b;
};

//1  赋值构造函数 用1个对象去初始化另外一个对象  
void main401()
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);

	//赋值=操作 会不会调用构造函数
	//operator=()//抛砖
	t0 = t1; //用t1 给 t0赋值  到操作 和 初始化是两个不同的概念

	//第1种调用方法
	Test4 t2 = t1; //用t1来初始化 t2   若explicit关键字声明则这种调用会报错
	t2.printT();

	cout << "hello..." << endl;
	system("pause");
	return;
}

//第二种调用时机
void main402()
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);

	Test4 t2(t1);  //用t1对象 初始化 t2对象 
	t2.printT();

	cout << "hello..." << endl;
	system("pause");
	return;
}