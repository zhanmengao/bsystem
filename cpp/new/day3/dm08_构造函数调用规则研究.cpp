
#include <iostream>
using namespace std;
class Test8
{
public:
	
// 	Test(const Test& obj) //copy构造函数 作用: 用一个对象初始化另外一个对象
// 	{
// 		a = obj.a + 100;
// 		b = obj.b + 100;
// 	}

// 	Test(int _a, int _b)
// 	{
// 		;
// 	}


	
	void printT()
	{
		cout << "a:" << a << "b: "<<b<< endl; 
	}

protected:
	Test8() = default;
private:
	int a;
	int b;
};

//当类中定义了拷贝构造函数时，c++编译器不会提供无参数构造函数
//当类中定义了有参数构造函数是,c++编译器不会提供无参数构造函数

//在定义类时, 只要你写了构造函数,则必须要用

void main801()
{
	//Test t1; //调用无参构造函数
	system("pause");
	return ;
}