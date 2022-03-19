#pragma once

//=============CAS无锁队列================
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

//构造函数
CQueue::CQueue()
{
	mfront = new Node();
	mrear = mfront;
}

//析构函数
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
*    思路：push操作我们在自旋体中首先不断更新mrear的值，然后进行CAS操作，
*    CAS操作将尾节点的next指针域置为新节点，然后我们将队列尾节点置为新节点。
*    这里要注意更新尾节点的写法：不能写为newNode，若此刻其它线程进入push，
*    程序将会造成错误。
*/
//插入元素
void CQueue::push(int val)
{
	Node* newNode = new Node(val);
	do
	{
		Node* ptmp = mrear;
		if (__sync_bool_compare_and_swap(&mrear->mpnext, nullptr, newNode))
		{
			mrear = mrear->mpnext; // 这里不能写为newNode，若此刻其它线程进入push，就会造成错误
			return;
		}
	} while (1); // 自旋

				 /*Node *newNode = new Node(val);
				 mrear->mpnext = newNode;
				 mrear = newNode;*/
}


/*
*    思路：pop操作我们首先要判断队列是否为空，若为空，我们直接返回即可。
不为空我们开始进行头删，那么同样的，在自旋体中我们不断更新pDel的值，
然后判空，再进行CAS操作删除结点，那么我们还需要注意一下若队列中只有
一个结点了，我们需要把mrear指向mfront。
*/
//删除一个元素
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
			if (pDel->mpnext == nullptr) // 队列中只剩一个结点
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

//打印函数
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
