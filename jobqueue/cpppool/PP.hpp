#pragma once
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>
using namespace std;


#define  MAX_THREAD_NUM 256

//线程池,可以提交变参函数或拉姆达表达式的匿名函数执行,可以获取执行返回值
//不支持类成员函数, 支持类静态成员函数或全局函数,Opteron()函数等
class threadpool_other
{
	using Task = std::function<void()>;
	// 线程池
	std::vector<std::thread> pool;
	// 任务队列
	std::queue<Task> tasks;
	// 同步
	std::mutex m_lock;
	// 条件阻塞
	std::condition_variable cv_task;
	// 是否关闭提交
	std::atomic<bool> stoped;
	//空闲线程数量
	std::atomic<int>  idlThrNum;

public:
	inline threadpool_other(unsigned short size = 4) :stoped{ false }
	{
		idlThrNum = size < 1 ? 1 : size;
		for (size = 0; size < idlThrNum; ++size)
		{  
			//初始化线程数量
			pool.emplace_back(
				[this]
			{ 
				// 工作线程函数
				while (!this->stoped)
				{
					std::function<void()> task;
					{   
						// 获取一个待执行的 task
						std::unique_lock<std::mutex> lock{ this->m_lock };// unique_lock 相比 lock_guard 的好处是：可以随时 unlock() 和 lock()
						this->cv_task.wait(lock,
							[this] {
							return this->stoped.load() || !this->tasks.empty();
						}
						); // wait 直到有 task
						if (this->stoped && this->tasks.empty())
							return;
						task = std::move(this->tasks.front()); // 取一个 task
						this->tasks.pop();
					}
					idlThrNum--;
					task();
					idlThrNum++;
				}
			}
			);
		}
	}
	inline ~threadpool_other()
	{
		stoped.store(true);
		cv_task.notify_all(); // 唤醒所有线程执行
		for (std::thread& thread : pool) 
		{
			//thread.detach(); // 让线程“自生自灭”
			if (thread.joinable())
				thread.join(); // 等待任务结束， 前提：线程一定会执行完
		}
	}

public:
	// 提交一个任务
	// 调用.get()获取返回值会等待任务执行完,获取返回值
	// 有两种方法可以实现调用类成员，
	// 一种是使用   bind： .commit(std::bind(&Dog::sayHello, &dog));
	// 一种是用 mem_fn： .commit(std::mem_fn(&Dog::sayHello), &dog)
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (stoped.load())    // stop == true ??
			throw std::runtime_error("commit on ThreadPool is stopped.");

		using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
		auto task = std::make_shared<std::packaged_task<RetType()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);    // wtf !
		std::future<RetType> future = task->get_future();
		{    
			// 添加任务到队列
			std::lock_guard<std::mutex> lock{ m_lock };
			tasks.emplace([task]()
			{ 
				(*task)();
			}
			);
		}
		cv_task.notify_one(); // 唤醒一个线程执行

		return future;
	}

	//空闲线程数量
	int idlCount() { return idlThrNum; }

};
#endif

void fun1(int slp)
{
	printf("  hello, fun1 !  %d\n", std::this_thread::get_id());
	if (slp > 0) {
		printf(" ======= fun1 sleep %d  =========  %d\n", slp, std::this_thread::get_id());
		std::this_thread::sleep_for(std::chrono::milliseconds(slp));
	}
}

struct gfun {
	int operator()(int n) {
		printf("%d  hello, gfun !  %d\n", n, std::this_thread::get_id());
		return 42;
	}
};

class A {
public:
	static int Afun(int n = 0) {   //函数必须是 static 的才能直接使用线程池
		std::cout << n << "  hello, Afun !  " << std::this_thread::get_id() << std::endl;
		return n;
	}

	static std::string Bfun(int n, std::string str, char c) {
		std::cout << n << "  hello, Bfun !  " << str.c_str() << "  " << (int)c << "  " << std::this_thread::get_id() << std::endl;
		return str;
	}
};

int PPmain()
{
	try
	{
		threadpool_other executor{ 50 };
		A a;
		std::future<void> ff = executor.commit(fun1, 0);
		std::future<int> fg = executor.commit(gfun{}, 0);
		std::future<int> gg = executor.commit(a.Afun, 9999);			 //IDE提示错误,但可以编译运行
		std::future<std::string> gh = executor.commit(A::Bfun, 9998, "mult args", 123);
		std::future<std::string> fh = executor.commit([]()->std::string { std::cout << "hello, fh !  " << std::this_thread::get_id() << std::endl; return "hello,fh ret !"; });

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(900));

		for (int i = 0; i < 50; i++) {
			executor.commit(fun1, i * 100);
		}
		std::cout << " =======  commit all ========= " << std::this_thread::get_id() << " idlsize=" << executor.idlCount() << std::endl;

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		ff.get(); //调用.get()获取返回值会等待线程执行完,获取返回值
		std::cout << fg.get() << "  " << fh.get().c_str() << "  " << std::this_thread::get_id() << std::endl;

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
		executor.commit(fun1, 55).get();    //调用.get()获取返回值会等待线程执行完

		std::cout << "end... " << std::this_thread::get_id() << std::endl;


		threadpool_other pool(4);
		std::vector< std::future<int> > results;

		for (int i = 0; i < 8; ++i)
		{
			results.emplace_back(
				pool.commit([i]
			{
				std::cout << "hello " << i << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "world " << i << std::endl;
				return i*i;
			})
			);
		}
		std::cout << " =======  commit all2 ========= " << std::this_thread::get_id() << std::endl;

		for (auto && result : results)
			std::cout << result.get() << ' ';
		std::cout << std::endl;
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << "some unhappy happened...  " << std::this_thread::get_id() << e.what() << std::endl;
	}
}