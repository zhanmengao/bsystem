#pragma once

//=============CAS��������================
#include <iostream>
#include <pthread.h>

using namespace std;

struct Node
{
	Node(int data = 0) :mdata(data), mpnext(nullptr) {}
	int mdata;
	Node* mpnext;
};

class CQueue
{
public:
	CQueue();
	~CQueue();
	void push(int val);
	void pop();
	void show();
private:
	Node* mfront;
	Node* mrear;
};

//���캯��
CQueue::CQueue()
{
	mfront = new Node();
	mrear = mfront;
}

//��������
CQueue::~CQueue()
{
	while (mfront != nullptr)
	{
		Node* pDel = mfront;
		mfront = mfront->mpnext;
		delete pDel;
	}
}

/*
*    ˼·��push���������������������Ȳ��ϸ���mrear��ֵ��Ȼ�����CAS������
*    CAS������β�ڵ��nextָ������Ϊ�½ڵ㣬Ȼ�����ǽ�����β�ڵ���Ϊ�½ڵ㡣
*    ����Ҫע�����β�ڵ��д��������дΪnewNode�����˿������߳̽���push��
*    ���򽫻���ɴ���
*/
//����Ԫ��
void CQueue::push(int val)
{
	Node* newNode = new Node(val);
	do
	{
		Node* ptmp = mrear;
		if (__sync_bool_compare_and_swap(&mrear->mpnext, nullptr, newNode))
		{
			mrear = mrear->mpnext; // ���ﲻ��дΪnewNode�����˿������߳̽���push���ͻ���ɴ���
			return;
		}
	} while (1); // ����

				 /*Node *newNode = new Node(val);
				 mrear->mpnext = newNode;
				 mrear = newNode;*/
}


/*
*    ˼·��pop������������Ҫ�ж϶����Ƿ�Ϊ�գ���Ϊ�գ�����ֱ�ӷ��ؼ��ɡ�
��Ϊ�����ǿ�ʼ����ͷɾ����ôͬ���ģ��������������ǲ��ϸ���pDel��ֵ��
Ȼ���пգ��ٽ���CAS����ɾ����㣬��ô���ǻ���Ҫע��һ����������ֻ��
һ������ˣ�������Ҫ��mrearָ��mfront��
*/
//ɾ��һ��Ԫ��
void CQueue::pop()
{
	Node* pDel = mfront->mpnext;
	if (pDel == nullptr)
	{
		return;
	}

	do
	{
		pDel = mfront->mpnext;

		if (pDel == nullptr)
		{
			return;
		}
		if (__sync_bool_compare_and_swap(&mfront->mpnext, pDel, pDel->mpnext))
		{
			if (pDel->mpnext == nullptr) // ������ֻʣһ�����
			{
				mfront = mrear;
			}
			delete pDel;
			return;
		}
	} while (1);

	/*Node *pDel = mfront->mpnext;
	if (pDel == nullptr)
	{
	return;
	}
	mfront->mpnext = pDel->mpnext;
	delete pDel;*/
}

//��ӡ����
void CQueue::show()
{
	Node* pCur = mfront->mpnext;
	while (pCur != nullptr)
	{
		cout << pCur->mdata << " ";
		pCur = pCur->mpnext;
	}
	cout << endl;
}

int main()
{
	CQueue queue;
	for (int i = 0; i < 20; i++)
	{
		int data = rand() % 101;
		queue.push(data);
	}

	queue.show();
	queue.pop();
	queue.pop();
	queue.pop();
	queue.show();
	return 0;
}
