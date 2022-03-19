#pragma once
#include"List.hpp"

#ifdef __cplusplus
//ջ������ȳ�
namespace stb
{
	//ͨ������ʵ�ֵ�ջ
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
		int top;				//ͷ������λ��
		T *mArr;				//����
		unsigned mSize;				//����
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
		//���ܣ��ж�ջ�� O(1)
		bool Empty()
		{
			return (this->top == -1);
		}

		void Push(const T& newnode)
		{
			if (IsFull())
			{
				//����
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
		//���ܣ���ջ O(1)
		//�����ջ��Ԫ��
		void Pop()
		{
			if (this->Empty())
			{
				//�׳��쳣
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
		//���ܣ�����ջ O(1)
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
		//���ܣ�����ջ O(1)
		void MakeNullStack()
		{
			ClearStack(0);
		}
	private:
		//���ܣ��ж�ջ��  O(1)
		bool IsFull()
		{
			return top == (mSize - 1);
		}
	};

	//��С����
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
