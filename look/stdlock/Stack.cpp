#include "Define.h"

/*
存在好几种情况，即使我们已经使用了互斥量，数据还是被破坏了。
①将被保护数据暴露到互斥量作用域之外
②被保护数据的访问接口本身就存在竞态条件（条件竞争）
*/
MutexTest mu;
std::stack<char> *pData;
//外部拿到了需要保护的数据的引用
void inject(std::stack<char> &data)
{
	pData = &data;
}
// 即使process没有显式传出，但是还是被inject传出
// process执行完后，pData能在无锁的情况下访问数据
void Test()
{
	//在上锁后执行了用户定义的函数，将被保护数据传递到互斥锁作用域之外
	mu.process(inject);
	std::stack<char> tmpStack;
	for (int i = 0; pData->size(); ++i)
	{
		auto &data = pData->top();
		data = 'a';
		pData->pop();
		tmpStack.push(data);
	}
	for (int i = 0; tmpStack.size(); ++i)
	{
		auto &data = tmpStack.top();
		pData->push(data);
		tmpStack.pop();
	}
}

//线程安全的函数
std::mutex stackMutex;
std::stack<char> intStack;
void SafeProcess()
{
	//即使top和pop已经是原子操作，但还是要加锁，因为被保护数据的访问接口本身就存在竞态条件
	std::lock_guard<std::mutex> guard(stackMutex);       
	if (!intStack.empty())
	{
		const int value = intStack.top();
		intStack.pop();
	}
}
void SafeMain()
{
	std::thread mutexTestThread1(Test);
	std::thread mutexTestThread2(Test);

	std::thread t1(SafeProcess);
	std::thread t2(SafeProcess);
	mutexTestThread1.join();
	mutexTestThread2.join();
	t1.join();
	t2.join();
}