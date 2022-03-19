
/*

#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void main()
{
	printf("hello...\n");
	system("pause");
	return ;
}
*/

#include "iostream" //包含c++的头文件 //iostream.h

using namespace std; //使用命名空间 std 标准的命名空间 (在这个命名空间中定义了很多标准定义)

void main101()
{
	//printf("hello....\n");

	//cout 标准输出 黑屏幕, 
	//<< 左移操作符  在c++里面 功能的改造(增强)===>C++ 语言操作符重载  //3day
	//endl \n
	//打印到屏幕 并且换行
	cout << "hello...."<<endl;

	system("pause");
}

//求圆的面积
//用面向过程的方法求解圆形的面积
// << >> 不会用: 标准输入cin和标准输出cout 始终写在操作符的左边 
void main102()
{
	double	r = 0;
	double	s = 0;

	cout<<"请输入圆形的半径:";

	//cin 标准输入 代表键盘
	cin >> r;
	cout<<"r的值是:" << r <<endl;
	s = 3.14*r*r;
	cout<<"圆形的面积是s:" <<s << endl;
	system("pause");
}

//在C++中 定义了一个自定义数据类型
struct Circle01
{
	double	m_s; //圆的面积
	double	m_r; //圆的半径
};

//在C++中 定义了一个自定义数据类型 MyCicle 圆形
class MyCicle
{
public:  
	double	m_s; //圆的面积 属性 成员变量
	double	m_r; //圆的半径

public:
	void setR(double r) //成员函数
	{
		m_r = r;
	}

	double getR() //成员函数
	{
		return m_r;
	}
	double getS()
	{
		m_s = 3.14*m_r*m_r;
		return m_s;
	}
};

//

void main103()
{
	MyCicle		c1, c2, c3; //用类 定义 变量 对象
	double		r;
	cout << "请输入c1圆形的半径:";
	cin >> r;

	//给c1圆形的属性赋值
	c1.setR(r);

	cout << "c1圆形的面积是:" << c1.getS()<<endl;

	system("pause");
}

//用面向对象的方法
//1 类的抽象 成员变量和成员函数 
//2 实例化 类的对象
//3 求面积
	//面向过程加工的是 一个一个的函数 
	//面向对象加工的是:一个一个的类
//4 main集成测试

//思考1:类的调用 执行过程分析==>类代码不是一步一步指向
//类是一个数据类型,(固定大小内存块的别名); 定义一个类,是一个抽象的概念,不会给你分配内存
//用数据类型定义变量的时候,才会分配内存
//思考2:抛砖: c++编译器是如何处理 多个对象,调用类的成员函数的;具体一点:如何区分是c1 c2 c3 调用了getS,抛砖.....

void main104()
{
	MyCicle		c1, c2, c3; //用类 定义 变量 对象
	double		r1, r2, r3;
	
	//圆形1
	cout << "请输入c1圆形的半径r1:";
	cin >> r1;
	//给c1圆形的属性赋值
	c1.setR(r1);
	cout << "c1圆形的面积是:" << c1.getS()<<endl;


	//圆形2
	cout << "请输入c2圆形的半径r2:";
	cin >> r2;
	//给c1圆形的属性赋值
	c2.setR(r2);
	cout << "c2圆形的面积是:" << c2.getS()<<endl;

	system("pause");
}






