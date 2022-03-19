

#include <iostream>
using namespace std;


class Test2
{
public:
	Test2()  //无参数构造函数
	{
		m_a = 0;
		m_b = 0;
		cout<<"无参数构造函数"<<endl;
	}
	
	Test2(int a)
	{
		m_a = a;
		m_b = 0;
	}

	Test2(int a, int b) //有参数构造函数   //3种方法
	{
		m_a = a;
		m_b = b;
		cout<<"有参数构造函数"<<endl;
	}

	//赋值构造函数 (copy构造函数) //explicit关键字将阻止这个构造被隐式调用
	Test2(const Test2& obj )
	{
		cout<<"我也是构造函数 " <<endl;
	}

public:
	void printT()
	{
		cout<<"普通成员函数"<<endl;
	}
private:
	int m_a;
	int m_b;
};

void main201()
{
	
	Test2 t1;  //调用无参数构造函数
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//调用 调用有参数构造函数 3
void main202()
{
	//1括号法 
	Test2 t1(1, 2);  //调用参数构造函数  c++编译器自动的调用构造函数
	t1.printT();

	// 2 =号法
	Test2 t2 = (3, 4, 5, 6, 7); // = c+对等号符 功能增强  c++编译器自动的调用构造函数 

	Test2 t3 = 5;

	//3 直接调用构造函数  手动的调用构造函数
	Test2 t4 = Test2(1, 2);   //匿名对象 (匿名对象的去和留) 抛砖 ....//t4对象的初始化

	t1 =  t4;  //把t4 copy给 t1  //赋值操作 
	//对象的初始化 和 对象的赋值 是两个不同的概念 
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}