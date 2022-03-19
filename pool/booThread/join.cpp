#include "Define.h"

/*
join()�ȴ��߳���ɣ�ֻ�ܶ�һ���̶߳������һ��join()����Ϊ����join()����Ϊ�����������߳�������ݣ�
����ٴε��ã������Runtime Error
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
��join()�ĵ��ã���Ҫѡ����ʵĵ���ʱ����
����߳�����֮���̲߳����쳣����join()����֮ǰ�׳�������ζ����ε��ûᱻ������
����취�ǣ������쳣�������ʹ��join()�������쳣��������е���join()
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
���沢�ǽ���������ĸ�������������������⵼�³�����ǰ�˳������淽���޽⣬��õķ�������ν��RAII��
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
	//��������
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

