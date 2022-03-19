#include "Define.h"

//在多线程编程中，有一个常见的情景是某个任务仅仅须要运行一次。在C++11中提供了非常方便的辅助类once_flag，call_once。

std::once_flag flag;

void do_once()
{
	std::call_once(flag, [](){ std::cout << "Called once" << std::endl; });
}

void MainCallOnce()
{
	std::thread t1(do_once);
	std::thread t2(do_once);
	std::thread t3(do_once);
	std::thread t4(do_once);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}


//值得注意的是，如果在函数执行中抛出了异常，那么会有另一个在once_flag上等待的线程会执行。

//实际上once_flag相当于一个锁，使用它的线程都会在上面等待，只有一个线程允许执行。
//如果该线程抛出异常，那么从等待中的线程中选择一个，重复上面的流程。
inline void may_throw_function(bool do_throw,int idx)
{
	// only one instance of this function can be run simultaneously
	if (do_throw) 
	{
		std::cout << "throw, index = "<<idx<<endl; // this message may be printed from 0 to 3 times
		// if function exits via exception, another function selected
		throw std::exception();
	}

	std::cout << "once,index = "<<idx<<endl; // printed exactly once, it's guaranteed that
	// there are no messages after it
}

inline void OnceFlag(bool do_throw,int idx)
{
	try 
	{
		std::call_once(flag, may_throw_function, do_throw,idx);
	}
	catch (...) 
	{
	}
}

void MainOnceFlag()
{
	std::thread t1(OnceFlag, true,1);
	std::thread t2(OnceFlag, true,2);
	std::thread t3(OnceFlag, false,3);
	std::thread t4(OnceFlag, true,4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}
