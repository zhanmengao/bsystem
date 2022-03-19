#include<iostream>
#include<thread>
#include<string>
#include<vector>
#include<list>
#include<mutex>

/*
����������Ĳ���ʱ����������ʱ�ͷ�����Ȼ����Ҫ����������ʱ�򣬿��Լ��������������������ظ���ʵ����lock_guard���󣬻��ܼ�����������
ͬ��������ʹ��std::defer_lock���ó�ʼ����ʱ�򲻽���Ĭ�ϵ���������

����ʹ�������ͱ�lock_guard������Ȼ����Ҳ���д��۵ģ���Ϊ���ڲ���Ҫά������״̬������Ч��Ҫ��lock_guard��һ�㣬��lock_guard�ܽ�������ʱ�򣬾�����lock_guard����֮��ʹ��unique_lock��

������ѧϰ����������ʱ�򣬻�����unique_lock������֮�ء�

���⣬��ע�⣬unique_lock��lock_guard�����ܸ��ƣ�lock_guard�����ƶ�������unique_lock���ԣ�
*/
using namespace std;

class A
{
public:
	void inMsgRecvQueue()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
			{
				std::unique_lock<std::mutex> sbguard(my_mutex, std::try_to_lock);
				if (sbguard.owns_lock())
				{
					//�õ�����
					msgRecvQueue.push_back(i);
					//...
					//�����������
				}
				else
				{
					//û�õ���
					cout << "inMsgRecvQueue()ִ�У���û�õ���ͷ��ֻ�ܸɵ�����" << i << endl;
				}
			}
		}
	}
	//��std::defer_lock��ǰ���ǣ����ܸ���һ���Ѿ�������mutex,����ᱨ�쳣
	//std::defer_lock����˼���ǲ�û�и�mutex��������ʼ����һ��û�м�����mutex��
	void inMsgRecvQueue_DeferLock()
	{
		for (int i = 0; i < 10000; i++)
		{
			cout << "inMsgRecvQueue()ִ�У�����һ��Ԫ��" << i << endl;
			std::unique_lock<std::mutex> sbguard(my_mutex, std::defer_lock);//û�м�����my_mutex
			sbguard.lock();//���ǲ����Լ�unlock
			//���������

			//��Ϊ��һЩ�ǹ������Ҫ����
			sbguard.unlock();
			//����ǹ������Ҫ��������

			sbguard.lock();
			//���������

			msgRecvQueue.push_back(i);
			//...
			//�����������
			sbguard.unlock();//�������㣬��Ҳ���ԣ����������Լ�������
		}
	}

	//���Ը�����������������ò�����������false,����õ�����������true,��������ǲ�������
	void inMsgRecvQueue_TryLock()
	{
		for (int i = 0; i < 10000; i++)
		{
			std::unique_lock<std::mutex> sbguard(my_mutex, std::defer_lock);//û�м�����my_mutex

			if (sbguard.try_lock() == true)//����true��ʾ�õ�����
			{
				msgRecvQueue.push_back(i);
				//...
				//�����������
			}
			else
			{
				//û�õ���
				cout << "inMsgRecvQueue()ִ�У���û�õ���ͷ��ֻ�ܸɵ�����" << i << endl;
			}
		}
	}

	//�������������mutex����ָ�룬���ͷ�����Ȩ��Ҳ����˵�����unique_lock��mutex�����й�ϵ��
	void inMsgRecvQueue_release()
	{
		for (int i = 0; i < 10000; i++)
		{
			std::unique_lock<std::mutex> sbguard(my_mutex);
			std::mutex *ptx = sbguard.release();			//�ͷ�ԭʼmutex

			msgRecvQueue.push_back(i);

			ptx->unlock();								//�Լ�����mutex��unlock��
		}
	}

	//����ֵ����ʱ����������ʽmove
	std::unique_lock<std::mutex> rtn_unique_lock()
	{
		std::unique_lock<std::mutex> tmpguard(my_mutex);
		return tmpguard;//�Ӻ����з���һ���ֲ���unique_lock�����ǿ��Եġ�����ʮ�Ľڽ�����ƶ����캯����
		//�������־ٱ�����tmpguard�ᵼ��ϵͳ������ʱunique_lock���󣬲�����unique_lock���ƶ����캯��
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
		my_mutex.lock();//Ҫ��lock(),����������unique_lock��std::adopt_lock����
		std::unique_lock<std::mutex> sbguard(my_mutex, std::adopt_lock);

		std::chrono::milliseconds dura(20000);
		std::this_thread::sleep_for(dura);  //��Ϣ20s

		if (!msgRecvQueue.empty())
		{
			//��Ϣ��Ϊ��
			int command = msgRecvQueue.front();//���ص�һ��Ԫ�أ��������Ԫ���Ƿ����
			msgRecvQueue.pop_front();//�Ƴ���һ��Ԫ�ء��������أ�

			return true;
		}
		return false;
	}
	//�����ݴ���Ϣ����ȡ�����߳�
	void outMsgRecvQueue()
	{
		int command = 0;
		for (int i = 0; i < 10000; i++)
		{
			bool result = outMsgLULProc(command);

			if (result == true)
			{
				cout << "outMsgRecvQueue()ִ�У�ȡ��һ��Ԫ��" << endl;
				//��������
			}
			else
			{
				//��Ϣ����Ϊ��
				cout << "inMsgRecvQueue()ִ�У���Ŀǰ��Ϣ������Ϊ�գ�" << i << endl;
			}
		}
		cout << "end!" << endl;
	}

private:
	std::list<int> msgRecvQueue;//��������Ϣ���У���������ҷ��͹��������
	std::mutex my_mutex;//����һ����������һ������
};

int Main_unique_lock()
{
	A myobja;

	std::thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
	std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);

	myOutMsgObj.join();
	myInMsgObj.join();

	cout << "���߳�ִ�У�" << endl;

	return 0;
}

