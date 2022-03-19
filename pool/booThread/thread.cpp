#include "Define.h"

//�����̶߳���ʱ��Ҫ�мǣ�ʹ��һ���ܷ��ʾֲ������ĺ���ȥ�����߳���һ������ע�⡣



/*
�Ժ�����ַΪ����
*/
void hello()
{
	std::cout << "Hello world, concurrency" << std::endl;
}
int mainFunc()
{
	std::thread t(hello);
	t.join(); // û����仰����Debug Error��
	return 0;
}

/*
�Ժ�������Ϊ����
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
����ĳ�Ա������Ϊ����
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
��lambda������Ϊ����
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
