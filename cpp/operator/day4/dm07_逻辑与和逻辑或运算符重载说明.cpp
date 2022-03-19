#include <cstdlib>
#include <iostream>

using namespace std;

class Test
{
	int i;
public:
	Test(int i)
	{
		this->i = i;
	}

	Test operator+ (const Test& obj)
	{
		Test ret(0);

		cout<<"执行+号重载函数"<<endl;
		ret.i = i + obj.i;
		return ret;
	}

	bool operator&& (const Test& obj)
	{
		cout<<"执行&&重载函数"<<endl;

		return i && obj.i;
	}
};

// && 从左向右
void main07()
{
	int a1 = 0;
	int a2 = 1;

	cout<<"注意：&&操作符的结合顺序是从左向右"<<endl;

	if( a1 && (a1 + a2) )
	{
		cout<<"有一个是假，则不在执行下一个表达式的计算"<<endl;
	}

	Test t1 = 0;
	Test t2 = 1;

	//if( t1 && (t1 + t2)  )
	//t1  && t1.operator+(t2)
	// t1.operator&&(  t1.operator+(t2) )   

	//1 && || 重载他们 不会产生短路效果
	if(  (t1 + t2) && t1)
	{
		//t1.operator+(t2) && t1;
		//(t1.operator+(t2)).operator&&(t1);
	
		cout<<"两个函数都被执行了，而且是先执行了+"<<endl;
	}

	//2 && 运算符的结合性
	// 两个逻辑与运算符  在一块的时候, 采去谈 运算符的结合性
	// 从左到右    (t1 + t2) && t1 ; 运算结果 && t2)
	//if(  (t1 + t2) && t1 && t2)
	{
		//t1.operator+(t2) && t1;
		//(t1.operator+(t2)).operator&&(t1);

		cout<<"两个函数都被执行了，而且是先执行了+"<<endl;
	}

	system("pause");
	return ;
}
