


#include <iostream>
using namespace std;


class Location6
{ 
public:
	Location6( int xx = 0 , int yy = 0 ) 
	{ 
		X = xx ;  Y = yy ;  cout << "Constructor Object.\n" ; 
	}

	//copy构造函数  完成对象的初始化
	Location6(const Location6 & obj) //copy构造函数 
	{
		X = obj.X; Y = obj.Y;
		cout << "Copy Create Object.\n";
	}
	Location6& operator=(const Location6& obj)
	{
		X = obj.X; Y = obj.Y;
		cout << "operator= Object.\n";
		return *this;
	}
	~Location6() 
	{ 
		cout << X << "," << Y << " Object destroyed." << endl ; 
	}
	int  GetX () { return X ; }		
	int GetY () { return Y ; }
private :   
	int  X , Y ;
} ;

//g函数 返回一个元素 
//结论1 : 函数的返回值是一个元素 (复杂类型的), 返回的是一个新的匿名对象(所以会调用匿名对象类的copy构造函数)

//
//结论2: 有关 匿名对象的去和留
//如果用匿名对象  初始化 另外一个同类型的对象, 匿名对象 转成有名对象
//如果用匿名对象  赋值给 另外一个同类型的对象, 匿名对象 被析构

//
//你这么写代码,设计编译器的大牛们:
//我就给你返回一个新对象(没有名字 匿名对象)
Location6 g()
{
	cout << "G Begin-----------------------------------" << endl;
	Location6 A(1, 2);
	cout << "G End-----------------------------------" << endl;
	return A;
}

//
void objplay2()
{
	g(); 
}

//
void objplay3()
{
	//用匿名对象初始化m 此时c++编译器 直接把匿名对转成m;(扶正) 从匿名转成有名字了m
	Location6 m = g(); 
	printf("匿名对象,被扶正,不会析构掉\n");
	cout<<m.GetX()<<endl;;
}

void objplay4()
{
	//用匿名对象 赋值给 m2后, 匿名对象被析构
	Location6 m2(1, 2);
	m2 = g();
	printf("因为用匿名对象=给m2, 匿名对象,被析构\n");
	//cout<<m2.GetX()<<endl;;
}
void main333()
{
	//objplay2();
	//objplay3();
	objplay4();
	cin.get();
}