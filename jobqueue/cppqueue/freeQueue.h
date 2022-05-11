#pragma once
#include<atomic>
#include<memory>





class FreeQueue
{
private:
	class FreeNode
	{
	public:
		FreeNode* next;
		int *value;
	};

	typedef FreeNode* NodePtr;
	typedef std::atomic<NodePtr> NodeAto;
public:
	FreeQueue()
	{
		NodePtr node(new FreeNode());
		node->next = nullptr;
		tail = node;
	}
public:
	void EnQueueV1(int* val)
	{
		NodePtr node(new FreeNode());
		node->value = val;
		node->next = nullptr;

		bool ok = false;
		do
		{
			NodeAto next(tail.load()->next);				//拿到最新tail的next指针
			ok = next.compare_exchange_weak(node->next, node);
		} while (!ok);
		//如果T1 线程还没有更新tail指针，其它的线程继续失败，因为tail->next不是NULL了。
		tail = node;
	}

	//解决V1问题：如果T1线程在用CAS更新tail指针的之前，线程停掉或是挂掉了，那么其它线程就进入死循环了
	void EnQueueV2(int *val)
	{
		NodePtr node(new FreeNode());
		node->value = val;
		node->next = nullptr;

		auto p = tail.load();
		auto oldp = p;
		bool ok = false;
		do
		{
			//我们让每个线程，自己fetch 指针 p 到链表尾
			while (p->next != nullptr)
				p = p->next;
			NodeAto next(p->next);
			ok = next.compare_exchange_strong(node->next, node);
		} while (!ok);
		tail = node;
	}
private:
	NodeAto tail;
};