
#include <iostream>
using namespace std;

//1 第一点 单独定义的引用时，必须初始化；说明很像一个常量
void main1001()
{
	//
	 const  int c1 = 10;

	 int a = 10;
	 int &b = a;  //b很想一个常量

	 printf("&a:%d \n", &a);
	 printf("&b:%d \n", &b); //===> a 和 b就是同一块内存空间的门牌号
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//2 普通引用有自己的空间吗？ 有 
struct Teacher
{
	char name[64]; //64
	int age; //4 

	int &a; //4 0  //很像指针 所占的内存空间大小
	int &b; //4 0 
};

//3 引用的本质

void modifyA(int &a1)
{
	a1 = 100;
}

void modifyA2(int * const a1)
{
	*a1 = 200;  //*实参的地址 ,去间接的修改实参的值 
}

void main1002()
{
	int a = 10;

	//1
	modifyA(a); //指向这个函数调用的时候,我们程序员不需要取a的地址
	printf("a:%d \n", a);

	//
	a = 10;
	modifyA2(&a); //如果是指针 需要我们程序员手工的取实参的地址 
	printf("a:%d \n", a); 

	printf("sizeof(Teacher):%d \n", sizeof(Teacher));
	system("pause");
}

void modifyA3(int *p)
{
	*p = 200;  //*p 3*p形参去间接修改实参的值
}
//间接赋值
void main213131()
{
	int a = 10;
	int *p = NULL; //间接赋值成立的三个条件 1 定义两个变量 

	p = &a;
	*p = 100;

	{
		*p = 200;
	}

	modifyA3(&a); //2 建立关联

}


// 123 写在一块
// 12   3
//1      23