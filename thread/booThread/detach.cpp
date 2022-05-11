#include "Define.h"


/*
detach()将子线程和父线程分离。
分离线程后，可以避免异常安全问题，即使线程仍在后台运行，分离操作也能确保std::terminate在std::thread对象销毁时被调用。
通常称分离线程为守护线程（deamon threads），这种线程的特点就是长时间运行；
线程的生命周期可能会从某一个应用起始到结束，可能会在后台监视文件系统，还有可能对缓存进行清理，亦或对数据结构进行优化。
*/
int maindetach()
{
	std::thread t([](std::string text)
	{
		std::cout << "hello world, " << text << std::endl;
	}, "lee");

	if (t.joinable())
	{
		//分离
		t.detach();
	}
	assert(!t.joinable());

	return 0;
}