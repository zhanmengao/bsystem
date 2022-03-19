#pragma once
#include"List.hpp"
#include"vector.hpp"
#include"Stack.h"
namespace stb
{
	template<class T>
	class Queue
	{
	private:
		List<T> list;
	public:
		Queue()
		{
		}
		void Clear()
		{
			list.Clear();
		}
		bool Empty() const
		{
			return list.Empty();
		}
		//输入
		void Push(const T& value)
		{
			list.PushBack(value);
		}
		//弹出
		void Pop()
		{
			return list.Remove(list.Begin());
		}
		T Front()
		{
                        return (list.Begin()->Data);
		}
		//通过栈逆置队列
		void RevertList()
		{
			Stack<int> stack;
			//入栈
			while (Empty())
			{
				stack.Push(Front());
				Pop();
			}
			//出队
			while (!stack.Empty())
			{
				Push(stack.Back());
				Pop();
			}
		}
	};

	template<class T>
	class QueuePlus
	{
	private:
		T* mArr;
		unsigned mArrSize;

		unsigned int front = 0;					//指向队头
		unsigned int rear = 0;					//指向队尾
		unsigned int capacity = 0;					//队列的元素个数
	public:
		QueuePlus():mArrSize(5)
		{
			mArr = new T[mArrSize];
		}
		~QueuePlus()
		{
			Clear();
		}
		void Clear()
		{
			if (mArr != nullptr)
			{
				delete[]mArr;
				mArr = nullptr;
			}
		}
		bool Empty()
		{
			return (front == rear);
		}
		void Push(const int& value)
		{
			if (IsFull())
			{
				//扩容
				T *arr = new T[mArrSize * 2];
				for (int i = 0; i < mArrSize; i++)
				{
					arr[i] = mArr[i];
				}
				//拷贝结束，整理扩容后数据（将放在数组前边的队尾全部扔到后边）
				if (rear < front)
				{
					for (int i = 0; i < front; i++)
					{
						arr[mArrSize + i] = arr[i];
					}
					rear += mArrSize;
				}
				mArrSize *= 2;
				delete[]mArr;
				mArr = arr;
			}
			mArr[rear] = value;
			//循环
			rear = (rear + 1) % mArrSize;
			capacity++;
		}
		void Pop()
		{
			front = (front + 1) % mArrSize;
			capacity--;
		}
		T Front() const
		{
			return mArr[front];
		}
		int Size() const
		{
			return capacity;
		}
		
	private:
		bool  IsFull() const
		{
			return (rear + 1) % mArrSize == front;
		}
	};

	class QueueByStack
	{
	private:
		Stack<int> mPush;
		Stack<int> mPop;
	public:
		void Push(const int& rData)
		{
			mPush.Push(rData);
		}
		void Pop()
		{
			CheckPopStack();
			mPop.Pop();
		}
		int Front()
		{
			CheckPopStack();
			mPop.Back();
		}
	private:
		void CheckPopStack()
		{
			if (mPop.Empty())
			{
				while (!mPush.Empty())
				{
					mPop.Push(mPush.Back());
					mPush.Pop();
				}
			}
		}
	};
	class StackByQueue
	{
	private:
		Queue<int> mPush;
		Queue<int> mPop;
	public:
		void Push(const int& rData)
		{
			mPush.Push(rData);
		}
		void Pop()
		{
			CheckPopQueue();
			mPop.Pop();
		}
		int Front()
		{
			CheckPopQueue();
			mPop.Front();
		}
	private:
		void CheckPopQueue()
		{
			if (mPop.Empty())
			{
				while (!mPush.Empty())
				{
					mPop.Push(mPush.Front());
					mPush.Pop();
				}
			}
		}
	};
}
