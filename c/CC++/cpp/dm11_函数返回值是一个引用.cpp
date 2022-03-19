
#include <iostream>
using namespace std;

int getAA1()
{
	int a ;
	a = 10;
	return a;
}

//返回a的本身 返回a的一个副本 10 
int& getAA2()
{
	int a ; //如果返回栈上的 引用, 有可能会有问题
	a = 10;
	return a;
}

int* getAA3()
{
	int a ;
	a = 10;
	return &a;
}

void main1101()
{
	int a1 = 0;
	int a2 = 0;
	a1 = getAA1();

	a2 = getAA2(); //10
	int &a3 = getAA2();  //若返回栈变量	不能成为其它引用的初始值

	printf("a1:%d \n", a1);
	printf("a2:%d \n", a2); 
	printf("a3:%d \n", a3);  // *a3

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//变量是static 或者是 全局变量

int j1()
{
	static int a = 10;
	a ++ ;
	return a;
}

int& j2()
{
	static int a = 10;
	a ++ ;
	return a;
}

//若返回静态变量或全局变量
//	可以成为其他引用的初始值
//	即可作为右值使用，也可作为左值使用
void main1102()
{
	int a1 = 10;
	int a2 = 20;

	a1 = j1();
	a2 = j2();
	int &a3 = j2();


	printf("a1:%d \n", a1);
	printf("a2:%d \n", a2); 
	printf("a3:%d \n", a3);  
	system("pause");
}


//////////////////////////////////////////////////////////////////////////
//--- 函数当左值
//返回变量的值
int g1()
{
	static int a = 10;
	a ++ ;
	return a;  //
} 

//返回变量本身 , 
int& g2()
{
	static int a = 10;
	a ++ ;
	printf("a:%d \n" , a);
	return a;
}

void main3213123()
{
	// g1() = 100;
	//11 = 100;

	g2() = 100; //函数返回值是一个引用,并且当左值 
	g2();

	int c1 = g1(); //函数返回值是一个引用,并且当右值 
	int c2 = g2(); //函数返回值是一个引用,并且当右值 
	//a = 100;
	system("pause");
}