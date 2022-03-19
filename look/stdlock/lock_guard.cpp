#include <iostream>       // std::cout
#include<mutex>
/*
只要将互斥量作为参数传递给std::lock()，std::lock()就能够锁住多个互斥量。std::lock()并未指定解锁和上锁的顺序，其能够保证：

std::lock()执行成功时，所有互斥量都已经被上锁，并且没有死锁问题
std::lock()执行失败时，已被其上锁的互斥量都会被解锁
*/
/*
lock_guard：
互斥锁保证了线程间的同步，但是却将并行操作变成了串行操作，这对性能有很大的影响，所以我们要尽可能的减小锁定的区域，也就是使用细粒度锁。

这一点lock_guard做的不好，不够灵活，lock_guard只能保证在析构的时候执行解锁操作，lock_guard本身并没有提供加锁和解锁的接口，
但是有些时候会有这种需求
*/
namespace Lock
{
	class some_big_object
	{
	public:
		some_big_object(int a) :x(a) {}
		void Print(){ std::cout << x << std::endl; }
	private:
		int x;
	};
	class TBigObj
	{
	private:
		some_big_object& some_detail;
		std::mutex m;
	public:
		TBigObj(some_big_object & sd) :some_detail(sd){}
		//std::lock()函数锁住两个互斥量，std::lock_guard负责unlock两个互斥量，如果不调用std::lock_guard()，需要手动unlock()。
		//std::adopt_lock参数表示互斥量已经上锁，这里仅仅是不会重复上锁。
		friend void swap(TBigObj& lhs, TBigObj& rhs)
		{
			if (&lhs == &rhs)
				return;
			std::lock(lhs.m, rhs.m);             //原子操作，统统上锁
			std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);         //在lock_guard变量析构后自动解锁
			std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);
			std::swap(lhs.some_detail, rhs.some_detail);
		}
	};

	template<class T>
	void swap(T& lhs, T& rhs);

	template<>
	void swap<some_big_object>(some_big_object &x, some_big_object &y)
	{
		TBigObj a(x), b(y);
		swap(a, b);       //调用TObj的swap
	}
}


int mainStdLock()
{
	Lock::some_big_object a(1), b(2);
	a.Print(), b.Print();
	Lock::swap<Lock::some_big_object>(a, b);       //调用someBigObj的swap
	a.Print(), b.Print();
	return 0;
}


