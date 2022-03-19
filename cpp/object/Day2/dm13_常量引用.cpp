
#include <iostream>
using namespace std;
//我们可以通过定义一个只含有引用的类来解决这个问题：

class refClass
{
private:
	double& ref;
public:
	refClass(double var = 42.0) :ref(var)
	{

	}
};

//所以结论就是引用和指针一样实际占内存空间4个字节。

//常引用的知识架构
void main1301()
{
	//普通引用

	int  a = 10;
	int &b = a;
	printf("b:%d \n", b);

	//常引用
	int  x = 20;
	const int &y = x;  //常引用 是 让变量 引用只读属性 不能通过y去修改x了
	//y = 21;

	//常引用 初始化 分为2种情况
	//1> 用变量 初始化 常引用
	{
		int x1 = 30;
		const int &y1 = x1; //用x1变量去初始化 常引用
	}
	//2> 用字面量 初始化 常量引用
	{
		const int a = 40;  //c++编译器把a放在符号表中
		//int &m = 41; //普通引用 引用一个字面量 请问字面量有没有内存地址
		//引用 就是给内存取多个门牌号 (多个别名)
		//printf("&40:%d \n", &40);
		const char &m = 43;  //c++编译器 会 分配内存空间 ，用printf实际得到的是变量本身的大小 而非引用的大小

	}
	cout <<"sizeof ref="<< sizeof refClass << endl;  // print 4
	system("pause");
	return ;
}



struct Teacher
{
	char name[64];
	int age ;
};


//void printTeacher(const Teacher  * const myt)
void printTeacher(const Teacher &myt)
{
	//常引用 让 实参变量 拥有只读属性 
	//myt.age = 33;
	printf("myt.age:%d \n", myt.age);
	
}

void main1302()
{
	Teacher  t1;
	t1.age = 36;

	printTeacher(t1);

	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}
