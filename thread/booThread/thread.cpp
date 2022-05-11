#include "Define.h"

//创建线程对象时需要切记，使用一个能访问局部变量的函数去创建线程是一个糟糕的注意。



/*
以函数地址为参数
*/
void hello()
{
	std::cout << "Hello world, concurrency" << std::endl;
}
int mainFunc()
{
	std::thread t(hello);
	t.join(); // 没有这句话，会Debug Error的
	return 0;
}

/*
以函数对象为参数
*/
class helloClass
{
public:
	helloClass()
	{ 
	}
	void operator()()const
	{
		std::cout << "Hello world" << std::endl;
	}
};
int mainClass()
{
	helloClass h;
    std::thread t1(h);
	t1.join();
	return 0;
}

/*
以类的成员函数作为参数
*/
class helloFunc
{
public:
	helloFunc(){ }
	void world1()
	{
		std::cout << "Hello world" << std::endl;
	}
	void world2(const std::string& text)
	{
		std::cout << "Hello world, " << text << std::endl;
	}
};

int mainMemberFunc()
{
    helloFunc h;
    std::thread t1(&helloFunc::world1, &h);
    std::thread t2(&helloFunc::world2, &h, "lee");
    t1.join();
    t2.join();
	return 0;
}

/*
以lambda对象作为参数
*/
int main()
{
	std::thread t([](std::string text)
	{
		std::cout << "hello world, " << text << std::endl;
	}, "lee");
	t.join();
	return 0;
}
