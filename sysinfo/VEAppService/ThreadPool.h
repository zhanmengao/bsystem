/*
==========================================================================
* ��ThreadPool�Ǳ�����ĺ����࣬�����Զ�ά���̳߳صĴ�����������е�����

* ���е�TaskFun��������
* ���е�TaskCallbackFun�ǻص�����

*�÷�������һ��ThreadPool������TaskFun������TaskCallbackFun�ص�������Ȼ�����ThreadPool��QueueTaskItem()��������

==========================================================================
*/
#pragma once
#include <Windows.h>
#include <list>
#include <queue>
#include <memory>

using std::list;
using std::queue;
using std::shared_ptr;

#define THRESHOLE_OF_WAIT_TASK  20

typedef int(*TaskFun)(SOCKET param);                // ������
typedef void(*TaskCallbackFun)(int result);        // �ص�����

class ThreadPool
{
private:
	// �߳���(�ڲ���)
	class Thread
	{
	public:
		Thread(ThreadPool *threadPool);
		~Thread();

		BOOL isBusy();                                                    // �Ƿ���������ִ��
		void ExecuteTask(TaskFun task, SOCKET param, TaskCallbackFun taskCallback);    // ִ������

	private:
		ThreadPool *threadPool;                                            // �����̳߳�
		BOOL    busy;                                                    // �Ƿ���������ִ��
		BOOL    exit;                                                    // �Ƿ��˳�
		HANDLE  thread;                                                    // �߳̾��
		TaskFun    task;                                                    // Ҫִ�е�����
		SOCKET    param;                                                    // �������
		TaskCallbackFun taskCb;                                            // �ص�������
		static unsigned int __stdcall ThreadProc(PVOID pM);                // �̺߳���
	};

	// IOCP��֪ͨ����
	enum WAIT_OPERATION_TYPE
	{
		GET_TASK,
		EXIT
	};

	// ��ִ�е�������
	class WaitTask
	{
	public:
		WaitTask(TaskFun task, SOCKET param, TaskCallbackFun taskCb, BOOL bLong)
		{
			this->task = task;
			this->param = param;
			this->taskCb = taskCb;
			this->bLong = bLong;
		}
		~WaitTask() { task = NULL; taskCb = NULL; bLong = FALSE; param = NULL; }

		TaskFun    task;                    // Ҫִ�е�����
		SOCKET param;                    // �������
		TaskCallbackFun taskCb;            // �ص�������
		BOOL bLong;                        // �Ƿ�ʱ������
	};

	// �������б�ȡ������̺߳���
	static unsigned int __stdcall GetTaskThreadProc(PVOID pM)
	{
		ThreadPool *threadPool = (ThreadPool *)pM;
		BOOL bRet = FALSE;
		DWORD dwBytes = 0;
		WAIT_OPERATION_TYPE opType;
		OVERLAPPED *ol;
		while (WAIT_OBJECT_0 != WaitForSingleObject(threadPool->stopEvent, 0))
		{
			BOOL bRet = GetQueuedCompletionStatus(threadPool->completionPort, &dwBytes, (PULONG_PTR)&opType, &ol, INFINITE);
			// �յ��˳���־
			if (EXIT == (DWORD)opType)
			{
				break;
			}
			else if (GET_TASK == (DWORD)opType)
			{
				threadPool->GetTaskExcute();
			}
		}
		return 0;
	}

	//�߳��ٽ�����
	class CriticalSectionLock
	{
	private:
		CRITICAL_SECTION cs;//�ٽ���
	public:
		CriticalSectionLock() { InitializeCriticalSection(&cs); }
		~CriticalSectionLock() { DeleteCriticalSection(&cs); }
		void Lock() { EnterCriticalSection(&cs); }
		void UnLock() { LeaveCriticalSection(&cs); }
	};


public:
	ThreadPool(size_t minNumOfThread = 2, size_t maxNumOfThread = 10);
	~ThreadPool();

	BOOL QueueTaskItem(TaskFun task, SOCKET param, TaskCallbackFun taskCb = NULL, BOOL longFun = FALSE);       // �������

private:
	size_t getCurNumOfThread() { return getIdleThreadNum() + getBusyThreadNum(); }    // ��ȡ�̳߳��еĵ�ǰ�߳���
	size_t GetMaxNumOfThread() { return maxNumOfThread - numOfLongFun; }            // ��ȡ�̳߳��е�����߳���
	void SetMaxNumOfThread(size_t size)            // �����̳߳��е�����߳���
	{
		if (size < numOfLongFun)
		{
			maxNumOfThread = size + numOfLongFun;
		}
		else
			maxNumOfThread = size;
	}
	size_t GetMinNumOfThread() { return minNumOfThread; }                            // ��ȡ�̳߳��е���С�߳���
	void SetMinNumOfThread(size_t size) { minNumOfThread = size; }                    // �����̳߳��е���С�߳���

	size_t getIdleThreadNum() { return idleThreadList.size(); }    // ��ȡ�̳߳��е��߳���
	size_t getBusyThreadNum() { return busyThreadList.size(); }    // ��ȡ�̳߳��е��߳���
	void CreateIdleThread(size_t size);                            // ���������߳�
	void DeleteIdleThread(size_t size);                            // ɾ�������߳�
	Thread *GetIdleThread();                                    // ��ȡ�����߳�
	void MoveBusyThreadToIdleList(Thread *busyThread);            // æµ�̼߳�������б�
	void MoveThreadToBusyList(Thread *thread);                    // �̼߳���æµ�б�
	void GetTaskExcute();                                        // �����������ȡ����ִ��
	WaitTask *GetTask();                                        // �����������ȡ����

	CriticalSectionLock idleThreadLock;                            // �����߳��б���
	list<Thread *> idleThreadList;                                // �����߳��б�
	CriticalSectionLock busyThreadLock;                            // æµ�߳��б���
	list<Thread *> busyThreadList;                                // æµ�߳��б�

	CriticalSectionLock waitTaskLock;
	list<WaitTask *> waitTaskList;                                // �����б�

	HANDLE                    dispatchThrad;                        // �ַ������߳�
	HANDLE                    stopEvent;                            // ֪ͨ�߳��˳���ʱ��
	HANDLE                    completionPort;                        // ��ɶ˿�
	size_t                    maxNumOfThread;                        // �̳߳��������߳���
	size_t                    minNumOfThread;                        // �̳߳�����С���߳���
	size_t                    numOfLongFun;                        // �̳߳�����С���߳���
};
