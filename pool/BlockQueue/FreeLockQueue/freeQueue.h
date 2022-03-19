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
			NodeAto next(tail.load()->next);				//�õ�����tail��nextָ��
			ok = next.compare_exchange_weak(node->next, node);
		} while (!ok);
		//���T1 �̻߳�û�и���tailָ�룬�������̼߳���ʧ�ܣ���Ϊtail->next����NULL�ˡ�
		tail = node;
	}

	//���V1���⣺���T1�߳�����CAS����tailָ���֮ǰ���߳�ͣ�����ǹҵ��ˣ���ô�����߳̾ͽ�����ѭ����
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
			//������ÿ���̣߳��Լ�fetch ָ�� p ������β
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