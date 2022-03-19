#pragma once
#include<new>

void mainNew()
{
	const int *pci = new const int(1024);				//new分配const对象合法

#ifdef _WIN32
	int *p2 = new (std::nothrow) int;			//定位new，不在内存耗尽的时候抛异常
    delete p2;
    p2 = nullptr;
    delete p2;				//delete空指针，没问题
#endif
	delete pci;

	int i1;
	//delete &i1;				//报错，delete了栈内存

}
