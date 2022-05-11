//=============CAS无锁栈================
#include <iostream>
#include <cstring>
using namespace std;

class CStack
{
public:
	CStack(int size);
	~CStack();
	void push(int& val);
	void pop();
	void show();
	bool empty()const;
	bool full()const;
private:
	int* mdata;
	int mtop;
	int msize;
};
CStack::CStack(int size = 10)
{
	msize = size;
	mtop = 0;
	mdata = new int[msize];
}

CStack::~CStack()
{
	delete[]mdata;
}



/*
*	思路：如果多个线程进行push，我们就要使用CAS操作
*	来保证我们对变量的互斥访问，首先在我们记录下mdata的地址
*	用于扩容前的CAS比较，然后我们进入push操作，在循环中
*	不断更新mtop的值，因为每个线程push都会改变mtop的值
*	请注意扩容操作中的if (mtop == msize)语句，是必不可少的
*	假如某线程正在执行扩容操作的mdata = pChange语句，还没有
*	执行msize *= 2，但此时有一个新线程进入了push函数，如果在
*	扩容前没有if (mtop == msize)语句，那么新线程又会再次扩容。
*/
void CStack::push(int& val)
{
	int* ptmp = mdata; /* ptmp 暂存 mdata*/
	while (1)
	{
		while (mtop >= msize) // 多线程自旋
		{
			if (ptmp != nullptr)
			{
				if (mtop == msize) // 最后两条语句被其他进入push的线程打断，防止多次扩容
				{
					if (__sync_bool_compare_and_swap(&mdata, ptmp, nullptr)) // CAS
					{
						int* pChange = new int[msize * 2];
						for (int i = 0; i < mtop; ++i)
						{
							pChange[i] = ptmp[i];
						}
						delete ptmp;
						mdata = pChange;
						msize *= 2;
					}
				}

			}
		}
		do
		{
			int tmp = mtop; // 循环拿到最新的栈顶元素
			if (mtop >= msize) // 多线程同时进行push，某个线程push后栈满
			{
				break;
			}
			if (__sync_bool_compare_and_swap(&mtop, tmp, tmp + 1)) // CAS
			{
				mdata[tmp] = val;
				return; // push over
			}

		} while (1); // 自旋
	}

	/*==========普通的push操作=========
		if (full())
		{
			int* ptmp = new int[msize * 2];
			for (int i = 0; i < msize; ++i)
			{
				ptmp[i] = mdata[i];
			}
			delete[]mdata;
			mdata = ptmp;
			msize *= 2;
		}
		mdata[mtop++] = val;
	*/
}

/*
*	思路：CAS实现pop操作较push容易一些，我们只需要在自旋循环中
*	不断更新mtop的值，然后进行CAS操作将mtop值减一即可，需要注意
*	的是多个线程pop时，有可能将栈中元素pop空，此时我们直接返回即可。
*/
void CStack::pop()
{
	if (empty())
	{
		return;
	}

	do
	{
		int tmp = mtop;
		if (tmp == 0)
		{
			break;
		}
		if (__sync_bool_compare_and_swap(&mtop, tmp, tmp - 1))// CAS ,满足条件将mtop更新为tmp - 1
		{
			return;
		}
	} while (1);


	// mtop--;
}

bool CStack::empty()const
{
	return mtop == 0;
}

bool CStack::full()const
{
	return mtop == msize;
}

void CStack::show()
{
	for (int i = 0; i < mtop; ++i)
	{
		cout << mdata[i] << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
	CStack stack;
	for (int i = 0; i < 20; ++i)
	{
		int data = rand() % 101;
		stack.push(data);
	}

	stack.show();
	stack.pop();
	stack.pop();
	stack.pop();
	stack.show();
}
