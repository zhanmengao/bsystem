#include "Define.h"

/*
join()等待线程完成，只能对一个线程对象调用一次join()，因为调用join()的行为，负责清理线程相关内容，
如果再次调用，会出现Runtime Error
*/
int mainjoin1()
{
	std::thread t([]()
	{
		std::cout << "hello world" << std::endl;
	});
	t.join(); // ok
	//t.join(); // runtime error
	if (t.joinable())
	{
		t.join(); // ok
	}
	return 0;
}

/*
对join()的调用，需要选择合适的调用时机。
如果线程运行之后父线程产生异常，在join()调用之前抛出，就意味着这次调用会被跳过。
解决办法是，在无异常的情况下使用join()——在异常处理过程中调用join()
*/
int mainTryjoin()
{
	std::thread t([](std::string text)
	{
		std::cout << "hello world, " << text << std::endl;
	}, "lee");
	try
	{
        throw std::logic_error("test");
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
		t.join();
	}
	if (t.joinable())
	{
		t.join();
	}
	return 0;
}

/*
上面并非解决这个问题的根本方法，如果其他问题导致程序提前退出，上面方案无解，最好的方法是所谓的RAII。
*/
class Thread
{
public:
    explicit Thread(std::thread &_t)
		: t(std::move(_t))
	{
		cout << "t.move()" << endl;
		if (!t.joinable())
			throw std::logic_error("No Thread");
	}

    ~Thread()
	{
		if (t.joinable())
		{
			t.join();
			cout << "t.join()" << endl;
		}
	}
	//不允许拷贝
    Thread(Thread const&) = delete;
    Thread& operator=(Thread const &) = delete;
private:
	std::thread t;
};

void func()
{
    std::thread t([](const std::string& text){
        std::cout << "hello world, " << text << std::endl;
    }, std::string("lee"));
    Thread guard(std::ref(t));
	try
	{
        throw std::exception();
	}
	catch (...)
	{
		throw;
	}
}

int mainMyThread()
{
	try
	{
		func();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}

