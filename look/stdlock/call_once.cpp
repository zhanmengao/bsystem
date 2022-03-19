#include "Define.h"

//�ڶ��̱߳���У���һ���������龰��ĳ�����������Ҫ����һ�Ρ���C++11���ṩ�˷ǳ�����ĸ�����once_flag��call_once��

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


//ֵ��ע����ǣ�����ں���ִ�����׳����쳣����ô������һ����once_flag�ϵȴ����̻߳�ִ�С�

//ʵ����once_flag�൱��һ������ʹ�������̶߳���������ȴ���ֻ��һ���߳�����ִ�С�
//������߳��׳��쳣����ô�ӵȴ��е��߳���ѡ��һ�����ظ���������̡�
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
