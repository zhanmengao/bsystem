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
		//����
		void Push(const T& value)
		{
			list.PushBack(value);
		}
		//����
		void Pop()
		{
			return list.Remove(list.Begin());
		}
		T Front()
		{
                        return (list.Begin()->Data);
		}
		//ͨ��ջ���ö���
		void RevertList()
		{
			Stack<int> stack;
			//��ջ
			while (Empty())
			{
				stack.Push(Front());
				Pop();
			}
			//����
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

		unsigned int front = 0;					//ָ���ͷ
		unsigned int rear = 0;					//ָ���β
		unsigned int capacity = 0;					//���е�Ԫ�ظ���
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
				//����
				T *arr = new T[mArrSize * 2];
				for (int i = 0; i < mArrSize; i++)
				{
					arr[i] = mArr[i];
				}
				//�����������������ݺ����ݣ�����������ǰ�ߵĶ�βȫ���ӵ���ߣ�
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
			//ѭ��
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
