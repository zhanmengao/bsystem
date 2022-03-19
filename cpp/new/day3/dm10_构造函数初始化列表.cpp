
#include <iostream>
using namespace std;

class A
{
public:
	A(int _a)
	{
		a = _a;
		cout << "构造函数" << "a" << a << endl;
	}

	~A()
	{
		cout << "析构函数" << "a" << a << endl;
	}

protected:
private:
	int a;
};


//1 构造函数的初始化列表  解决: 在B类中 组合了一个 A类对象 (A类设计了构造函数)
//根据构造函数的调用规则 设计A的构造函数, 必须要用;没有机会初始化A
//新的语法  Constructor::Contructor() : m1(v1), m2(v1,v2), m3(v3)
class B
{
public:
	B(int _b1, int _b2) : a1(1), a2(2), c(0)
	{

	}

	B(int _b1, int _b2, int m, int n) : a1(m), a2(n), c(0)
	{
		b1 = _b1;
		b2 = _b2;
		cout <<"B的构造函数"<<endl;
	}
	~B()
	{
		cout<<"B的析构函数" <<endl;
	}

protected:
private:
	int b1;
	int b2;
	A a2;
	A a1;
	const int c;
};


//2 先执行 被组合对象的构造函数 
//如果组合对象有多个,按照定义顺序, 而不是按照初始化列表的顺序

//析构函数 : 和构造函数的调用顺序相反

//3 被组合对象的构造顺序 与定义顺序有关系 ,与初始化列表的顺序没有关系.
//4 初始化列表 用来 给const 属性赋值 
//5 在初始化列表中对成员变量进行初始化才是真正的初始化，在构造函数中进行的只是赋值 -》在初始化列表中进行初始化效率会高
void obj10play()
{
	
	//A a1(10);
	//B ojbB(1, 2);

	//1参数传递 
	B ojbB2(1, 2,3, 4);

	//2 调用顺序
	
	return ;
}

void main100()
{
	obj10play();
	system("pause");
}