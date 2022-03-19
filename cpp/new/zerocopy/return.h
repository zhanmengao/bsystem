#pragma once
#include"move.h"

A pppp()
{
	A a;
	return a;
}

A ppmove()
{
	A a;
	return std::move(a);
}


class ABCD {
public:
	ABCD(int a, int b)
	{
		cout << "构造" << endl;
	}
	~ABCD()
	{
		cout << "析构" << endl;

	}
	ABCD(ABCD &obj)
	{
		cout << "拷贝构造" << endl;
	}
private:
	int a;
	int b;
};
ABCD pppp1()
{
	ABCD a(1, 2);
	printf("build in %s \n", __FUNCTION__);
	return a;
}

int mainReturn()
{
	ABCD a1 = pppp1();			//拷贝构造一个返回值给到a1
	printf("\n");
	return 0;
}
int mainReturn3()
{	
	A a = pppp();						//移动构造：这段代码会触发RVO
	printf("\n");
	return 0;
}


int mainReturn1() {
	A &&b = pppp();		//不会触发RVO，拷贝构造了一个临时的对象，临时对象的生命周期和rval_ref绑定
	//和上面等价
	const A& rval_ref = pppp();
	return 0;
}

int mainReturn2() {
	//和示例1类似，std::move一个临时对象是没有必要的，也会忽略掉返回值优化。
	A && c = ppmove();
	return 0;
}