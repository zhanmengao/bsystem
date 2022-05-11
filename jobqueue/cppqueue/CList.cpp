//=============CAS无锁链表================
#include <iostream>
using namespace std;

struct Node
{
	Node(int data = 0)
		:mdata(data), mpnext(nullptr) {};
	int mdata;
	Node* mpnext;
};

class CLink
{
public:
	CLink();
	~CLink();

	void insertHead(int val);
	void insertTail(int val);

	void removeNode(int val);
	void show();

	CLink(const CLink& link) = delete;
	CLink& operator=(const CLink& link) = delete;
private:
	Node* mpHead;
};

CLink::CLink()
{
	mpHead = new Node();
}

CLink::~CLink()
{
	Node* pCur = mpHead;
	while (pCur != nullptr)
	{
		mpHead = mpHead->mpnext;
		delete pCur;
		pCur = mpHead;
	}
}

void CLink::insertHead(int val)
{
	Node* newNode = new Node(val);
	do
	{
		Node* ptmp = newNode;
		if (__sync_bool_compare_and_swap(&newNode->mpnext, ptmp->mpnext, mpHead->mpnext))
		{
			mpHead->mpnext = ptmp;
			return;
		}
	} while (1);
}

void CLink::insertTail(int val)
{
	Node* newNode = new Node(val);
	Node* pCur = mpHead;
	do
	{
		Node* ptmp = newNode;
		Node* ptmpCur = pCur;
		if (__sync_bool_compare_and_swap(&pCur, ptmpCur, ptmpCur))
		{
			while (pCur->mpnext != nullptr)
			{
				Node* ptmploop = pCur;
				if (__sync_bool_compare_and_swap(&pCur, ptmploop, pCur->mpnext));
			}
			if (__sync_bool_compare_and_swap(&pCur, ptmpCur, ptmpCur))
			{
				pCur->mpnext = ptmp;
				return;
			}

		}
	} while (1);
}

void CLink::removeNode(int val) // 删除值为val的所有节点
{
	Node* pCur = mpHead->mpnext;
	Node* pPri = mpHead;

	do
	{
		Node* ptmp = pCur;
		if (__sync_bool_compare_and_swap(&pCur, ptmp, ptmp))
		{
			while (pCur != nullptr)
			{
				ptmp = pCur;
				Node* ptmpPri = pPri;
				if (pCur->mdata == val && pCur->mpnext != nullptr)
				{
					if (__sync_bool_compare_and_swap(&pCur->mdata, ptmp->mdata, pCur->mpnext->mdata))
					{
					}
					if (__sync_bool_compare_and_swap(&pCur->mpnext, ptmp->mpnext, pCur->mpnext->mpnext))
					{
						ptmp = pCur;
					}
					continue;
				}
				else if (pCur->mdata == val && pCur->mpnext == nullptr)
				{
					if (__sync_bool_compare_and_swap(&pPri->mpnext, ptmpPri->mpnext, nullptr))
					{
					}
					return;
				}
				Node* ptmploop = pCur;
				if (__sync_bool_compare_and_swap(&pCur, ptmploop, pCur->mpnext));

				Node* pPriloop = pPri;
				if (__sync_bool_compare_and_swap(&pPri, pPriloop, pPri->mpnext));

			}
		}
	} while (1);

}

void CLink::show()
{
	Node* pCur = mpHead->mpnext;
	while (pCur != nullptr)
	{
		cout << pCur->mdata << " ";
		pCur = pCur->mpnext;
	}
	cout << endl;
}

int main()
{
	CLink link;
	for (int i = 0; i < 10; i++)
	{
		int data = rand() % 101;
		link.insertHead(data);
	}

	for (int i = 0; i < 5; i++)
	{
		int data = rand() % 101;
		link.insertTail(data);
	}

	link.insertTail(100);
	link.insertTail(9);
	link.insertTail(8);
	link.insertHead(100);
	link.insertHead(100);
	link.insertTail(100);
	link.show();

	link.removeNode(100);
	link.show();
}
