
#include <iostream>
using namespace std;


//0 const的基础知识
struct Teacher
{
	char name[64];
	int age;
};

//指针所指向的内存空间,不能被修改
int  operatorTeacher01(const Teacher *pT)
{
	//pT->age = 10;
	return 0;
}

//指针变量本身不能被修改
int  operatorTeacher02( Teacher * const pT)
{
	pT->age = 10;
	//pT = NULL; //
	return 0;
}

int  operatorTeacher03( const Teacher * const pT)
{
	//pT->age = 10;
	//pT = NULL; //
	printf("age:%d", pT->age);
	return 0;
}


void main81()
{
// 	const int a;
// 	int const b; //一样
// 
// 	const int *c;  //const修饰的是指针所指向的内存空间,不能被修改
// 	int * const d; //
// 	const int * const e ;
	cout<<"hello..."<<endl;


	Teacher t1;
	t1.age = 33;
	operatorTeacher03(&t1);

	system("pause");
	return ;
}

//1
//c语言中的const是一个冒牌货
//C++语言中 const是一个真正的常量 

//2 原因分析
//const int b = 10; 符号表

void main82()
{
	//好像 a 是一个常量
	const int a = 10;
	//a = 11;
	int *p = NULL;
	p = (int *)&a;

	*p = 20;  //间接赋值 

	printf("a:%d \n", a);
	printf("*p:%d \n", *p);
	system("pause");
}

//3 const分配内存的时机 编译器编译器期间 
void main83()
{

	//好像 a 是一个常量
	int a;
	const int b = 10;
	int c; 

	printf("&a:%d, &b:%d, &c:%d \n", &a, &b, &c);
	
	system("pause");
	


}

//4const 和 #define的相同之处
//#define, 在编译预处理阶段 处理
//const常量是由编译器处理的，提供类型检查和作用域检查
#define d  20
void main84()
{
	//int a = 10;
	//int b = 20;
	//int array[a+b]; //linux内核里面是成立的;原因 编译linux内核的gcc编译器支持.
					//c和c++编译器 不支持这种语法现象 

	const int c = 10;
	//const int d = 20;
	int array2[c+d]; 

	system("pause");

}

//5 const定义的变量,由编译器处理的，提供类型检查和作用域检查
void fun1()
{
	#define a 10
	const int b = 20;
	//#undef  a 
	//# undef
}

void fun2()
{
	printf("a = %d\n", a);
	//printf("b = %d\n", b);
}

int main1111()
{
	fun1();
	fun2();
	return 0;
}




