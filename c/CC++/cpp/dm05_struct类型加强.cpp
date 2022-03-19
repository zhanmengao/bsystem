
#include <iostream>
using namespace std;

//struct 关键字 class关键字 完成的功能是一样的
//区别后面介绍 抛砖
class  c1
{
public:
protected:
private:
};
struct Teacher
{
public:
	char name[32];
	int age;
protected:
		int a;
};

void main501()
{
	Teacher t1; //
	t1.age = 10;
	printf("hello...\n");

	system("pause");
}


/*
C++中所有的变量和函数都必须有类型
	C语言中的默认类型在C++中是不合法的


函数f的返回值是什么类型，参数又是什么类型？
函数g可以接受多少个参数？
*/

/*
//更换成.cpp试试

f(i)
{
	printf("i = %d\n", i);

}

g()
{
	return 5;
}

int main(int argc, char *argv[])
{

	f(10);

	printf("g() = %d\n", g(1, 2, 3, 4, 5));


	getchar();	
	return 0;
}
*/



