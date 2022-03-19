#pragma once
#include <iostream>
//std::unique_lock
#include <mutex> 
#include <shared_mutex>
#include <thread>

class ThreadSafeCounter
{
public:
	ThreadSafeCounter() = default;

	// 多个线程/读者能同时读计数器的值。
	unsigned int get() const
	{
		std::shared_lock<std::shared_mutex> lock(mutex_);
		return value_;
	}

	// 只有一个线程/写者能增加/写线程的值。
	void increment()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		value_++;
	}

	// 只有一个线程/写者能重置/写线程的值。
	void reset()
	{
		std::unique_lock<std::shared_mutex> lock(mutex_);
		value_ = 0;
	}

private:
	mutable std::shared_mutex mutex_;
	unsigned int value_ = 0;
};

int mainSharedMutex()
{
	ThreadSafeCounter counter;

	auto increment_and_print = [&counter]()
	{
		for (int i = 0; i < 3; i++)
		{
			counter.increment();
			std::cout << std::this_thread::get_id() << '\t' << counter.get() << std::endl;
		}
	};

	std::thread thread1(increment_and_print);
	std::thread thread2(increment_and_print);

	thread1.join();
	thread2.join();

	system("pause");
	return 0;
}