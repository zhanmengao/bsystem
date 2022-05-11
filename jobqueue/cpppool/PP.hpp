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

//�̳߳�,�����ύ��κ�������ķ����ʽ����������ִ��,���Ի�ȡִ�з���ֵ
//��֧�����Ա����, ֧���ྲ̬��Ա������ȫ�ֺ���,Opteron()������
class threadpool_other
{
	using Task = std::function<void()>;
	// �̳߳�
	std::vector<std::thread> pool;
	// �������
	std::queue<Task> tasks;
	// ͬ��
	std::mutex m_lock;
	// ��������
	std::condition_variable cv_task;
	// �Ƿ�ر��ύ
	std::atomic<bool> stoped;
	//�����߳�����
	std::atomic<int>  idlThrNum;

public:
	inline threadpool_other(unsigned short size = 4) :stoped{ false }
	{
		idlThrNum = size < 1 ? 1 : size;
		for (size = 0; size < idlThrNum; ++size)
		{  
			//��ʼ���߳�����
			pool.emplace_back(
				[this]
			{ 
				// �����̺߳���
				while (!this->stoped)
				{
					std::function<void()> task;
					{   
						// ��ȡһ����ִ�е� task
						std::unique_lock<std::mutex> lock{ this->m_lock };// unique_lock ��� lock_guard �ĺô��ǣ�������ʱ unlock() �� lock()
						this->cv_task.wait(lock,
							[this] {
							return this->stoped.load() || !this->tasks.empty();
						}
						); // wait ֱ���� task
						if (this->stoped && this->tasks.empty())
							return;
						task = std::move(this->tasks.front()); // ȡһ�� task
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
		cv_task.notify_all(); // ���������߳�ִ��
		for (std::thread& thread : pool) 
		{
			//thread.detach(); // ���̡߳���������
			if (thread.joinable())
				thread.join(); // �ȴ���������� ǰ�᣺�߳�һ����ִ����
		}
	}

public:
	// �ύһ������
	// ����.get()��ȡ����ֵ��ȴ�����ִ����,��ȡ����ֵ
	// �����ַ�������ʵ�ֵ������Ա��
	// һ����ʹ��   bind�� .commit(std::bind(&Dog::sayHello, &dog));
	// һ������ mem_fn�� .commit(std::mem_fn(&Dog::sayHello), &dog)
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (stoped.load())    // stop == true ??
			throw std::runtime_error("commit on ThreadPool is stopped.");

		using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, ���� f �ķ���ֵ����
		auto task = std::make_shared<std::packaged_task<RetType()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);    // wtf !
		std::future<RetType> future = task->get_future();
		{    
			// ������񵽶���
			std::lock_guard<std::mutex> lock{ m_lock };
			tasks.emplace([task]()
			{ 
				(*task)();
			}
			);
		}
		cv_task.notify_one(); // ����һ���߳�ִ��

		return future;
	}

	//�����߳�����
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
	static int Afun(int n = 0) {   //���������� static �Ĳ���ֱ��ʹ���̳߳�
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
		std::future<int> gg = executor.commit(a.Afun, 9999);			 //IDE��ʾ����,�����Ա�������
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

		ff.get(); //����.get()��ȡ����ֵ��ȴ��߳�ִ����,��ȡ����ֵ
		std::cout << fg.get() << "  " << fh.get().c_str() << "  " << std::this_thread::get_id() << std::endl;

		std::cout << " =======  sleep ========= " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(3));

		std::cout << " =======  fun1,55 ========= " << std::this_thread::get_id() << std::endl;
		executor.commit(fun1, 55).get();    //����.get()��ȡ����ֵ��ȴ��߳�ִ����

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