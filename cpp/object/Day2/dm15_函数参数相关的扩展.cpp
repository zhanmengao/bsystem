
#include <iostream>
using namespace std;

void myPrint15(int x = 3)
{
	cout<<"x"<<x<<endl;
}

//1 若 你填写参数,使用你填写的,不填写默认
//2 在默认参数规则 ，如果默认参数出现，那么右边的都必须有默认参数
void myPrint2( int m, int n, int x = 3, int y = 4)
//void myPrint2( int m, int n, int x = 3, int y )
{
	cout<<"x"<<x<<endl;
}
//默认参数
void main1501()
{
	
	myPrint15(4);
	myPrint15();

	//
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//函数占位参数 函数调用是,必须写够参数

void func1_15(int a, int b, int)
{
	cout<<"a"<<a<<" b"<<b<<endl;
}
//占位参数
void main1502()
{
	
	//func1(1, 2); //err调用不起来
	func1_15(1, 2, 3);

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//默认参数和占位参数

void  func2_15(int a, int b, int =0)
{
	cout<<"a="<<a<<";b="<<b<<endl;
}

void main1503()
{
	func2_15(1, 2); //0k
	func2_15(1, 2, 3); //ok
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}