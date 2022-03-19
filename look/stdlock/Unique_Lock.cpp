#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<list>
#include<mutex>

/*
在无需加锁的操作时，可以先临时释放锁，然后需要继续保护的时候，可以继续上锁，这样就无需重复的实例化lock_guard对象，还能减少锁的区域。
同样，可以使用std::defer_lock设置初始化的时候不进行默认的上锁操作

这样使用起来就比lock_guard更加灵活！然后这也是有代价的，因为它内部需要维护锁的状态，所以效率要比lock_guard低一点，在lock_guard能解决问题的时候，就是用lock_guard，反之，使用unique_lock。

后面在学习条件变量的时候，还会有unique_lock的用武之地。

另外，请注意，unique_lock和lock_guard都不能复制，lock_guard不能移动，但是unique_lock可以！
*/
using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
			{
				std::unique_lock<std::mutex> sbguard(my_mutex, std::try_to_lock);
				if (sbguard.owns_lock())
				{
					//拿到了锁
					msgRecvQueue.push_back(i);
					//...
					//其他处理代码
				}
				else
				{
					//没拿到锁
					cout << "inMsgRecvQueue()执行，但没拿到锁头，只能干点别的事" << i << endl;
				}
			}
		}
	}
	//用std::defer_lock的前提是，不能给他一个已经加锁的mutex,否则会报异常
	//std::defer_lock的意思就是并没有给mutex加锁：初始化了一个没有加锁的mutex。
	void inMsgRecvQueue_DeferLock()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgRecvQueue()执行，插入一个元素" << i << endl;
			std::unique_lock<std::mutex> sbguard(my_mutex, std::defer_lock);//没有加锁的my_mutex
			sbguard.lock();//咱们不用自己unlock
			//处理共享代码

			//因为有一些非共享代码要处理
			sbguard.unlock();
			//处理非共享代码要处理。。。

			sbguard.lock();
			//处理共享代码

			msgRecvQueue.push_back(i);
			//...
			//其他处理代码
			sbguard.unlock();//画蛇添足，但也可以（对象析构自己解锁）
		}
	}

	//尝试给互斥量加锁，如果拿不到锁，返回false,如果拿到了锁，返回true,这个函数是不阻塞的
	void inMsgRecvQueue_TryLock()
	{
		for (int i = 0; i < 10000; i++)
		{
			std::unique_lock<std::mutex> sbguard(my_mutex, std::defer_lock);//没有加锁的my_mutex

			if (sbguard.try_lock() == true)//返回true表示拿到锁了
			{
				msgRecvQueue.push_back(i);
				//...
				//其他处理代码
			}
			else
			{
				//没拿到锁
				cout << "inMsgRecvQueue()执行，但没拿到锁头，只能干点别的事" << i << endl;
			}
		}
	}

	//返回它所管理的mutex对象指针，并释放所有权；也就是说，这个unique_lock和mutex不再有关系。
	void inMsgRecvQueue_release()
	{
		for (int i = 0; i < 10000; i++)
		{
			std::unique_lock<std::mutex> sbguard(my_mutex);
			std::mutex *ptx = sbguard.release();			//释放原始mutex

			msgRecvQueue.push_back(i);

			ptx->unlock();								//自己负责mutex的unlock了
		}
	}

	//返回值，临时变量发生隐式move
	std::unique_lock<std::mutex> rtn_unique_lock()
	{
		std::unique_lock<std::mutex> tmpguard(my_mutex);
		return tmpguard;//从函数中返回一个局部的unique_lock对象是可以的。三章十四节讲解过移动构造函数。
		//返回这种举报对象tmpguard会导致系统生成临时unique_lock对象，并调用unique_lock的移动构造函数
	}

	void inMsgRecvQueue_Move()
	{
		for (int i = 0; i < 10000; i++)
		{
			std::unique_lock<std::mutex> sbguard1 = rtn_unique_lock();

			msgRecvQueue.push_back(i);
		}
	}

	bool outMsgLULProc(int &command)
	{
		my_mutex.lock();//要先lock(),后续才能用unique_lock的std::adopt_lock参数
		std::unique_lock<std::mutex> sbguard(my_mutex, std::adopt_lock);

		std::chrono::milliseconds dura(20000);
		std::this_thread::sleep_for(dura);  //休息20s

		if (!msgRecvQueue.empty())
		{
			//消息不为空
			int command = msgRecvQueue.front();//返回第一个元素，但不检查元素是否存在
			msgRecvQueue.pop_front();//移除第一个元素。但不返回；

			return true;
		}
		return false;
	}
	//把数据从消息队列取出的线程
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = outMsgLULProc(command);

			if (result == true)
			{
				cout << "outMsgRecvQueue()执行，取出一个元素" << endl;
				//处理数据
			}
			else
			{
				//消息队列为空
				cout << "inMsgRecvQueue()执行，但目前消息队列中为空！" << i << endl;
			}
		}
		cout << "end!" << endl;
	}

private:
	std::list<int> msgRecvQueue;//容器（消息队列），代表玩家发送过来的命令。
	std::mutex my_mutex;//创建一个互斥量（一把锁）
};

int Main_unique_lock()
{
	A myobja;

	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myOutMsgObj.join();
	myInMsgObj.join();

	cout << "主线程执行！" << endl;

	return 0;
}

