#pragma once
#include"List.hpp"

#ifdef __cplusplus
//栈，后进先出
namespace stb
{
	//通过链表实现的栈
	template<class T>
	class Stack
	{
		List<T> mData;
	public:
		void Push(const T& rData)
		{
			mData.PushBack(rData);
		}
		T Back()
		{
                        return (mData.End()->Data);
		}
		void Pop()
		{
			mData.Remove(mData.End());
		}
		bool Empty() const
		{
			return mData.Empty();
		}
	};

	template<class T>
	class StackPlus
	{
	private:
		int top;				//头部索引位置
		T *mArr;				//容器
		unsigned mSize;				//容量
	public:
		StackPlus(unsigned size = 0) :mSize(size),top(-1)
		{
			if (mSize == 0)
			{
				mSize = 5;
			}
			mArr = new T[mSize];
		}
		~StackPlus()
		{
			if (mArr != nullptr)
			{
				delete[]mArr;
				mArr = nullptr;
			}
		}
		//功能：判断栈空 O(1)
		bool Empty()
		{
			return (this->top == -1);
		}

		void Push(const T& newnode)
		{
			if (IsFull())
			{
				//扩容
				T *arr = new T[mSize * 2];
				for (int i = 0; i < mSize; i++)
				{
					arr[i] = mArr[i];
				}
				delete[]mArr;
				mArr = arr;
				mSize *= 2;
			}
			mArr[++top] = newnode;
		}
		//功能：出栈 O(1)
		//输出：栈顶元素
		void Pop()
		{
			if (this->Empty())
			{
				//抛出异常
				throw new std::length_error("Stack pop");
			}
			top--;
		}
		T Front() const
		{
			return mArr[top];
		}
		unsigned Size() const
		{
			return top + 1;
		}
		//功能：重置栈 O(1)
		void ClearStack(unsigned newsize)
		{
			if (mArr != nullptr)
			{
                delete mArr;
				mArr = nullptr;
			}
			if (newsize > 0)
			{
				mArr = new T[newsize];
			}
			mSize = newsize;
			top = -1;
		}
		//功能：销毁栈 O(1)
		void MakeNullStack()
		{
			ClearStack(0);
		}
	private:
		//功能：判断栈满  O(1)
		bool IsFull()
		{
			return top == (mSize - 1);
		}
	};

	//最小队列
	template<class T>
	class MinStack
	{
	private:
		StackPlus<T> mData;
		StackPlus<T> mMin;
	public:
		void Push(const T& rData)
		{
			mData.Push(rData);
			if (mMin.Size() <= 0 || mMin.Front() > rData)
			{
				mMin.Push(rData);
			}
			else
			{
				mMin.Push(mMin.Front());
			}
		}
		void Pop()
		{
			mData.Pop();
			mMin.Pop();
		}
		T GetMin() const
		{
			return mMin.Front();
		}
	};
}




#endif
