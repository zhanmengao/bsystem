#ifndef _VECTOR_HPP
#define _VECTOR_HPP
#pragma once
#ifdef __cplusplus
#include"LinerList.h"
#include<stdexcept>
#include"List.hpp"
namespace stb
{
	template<class T>
	class Vector :public LinearList<T>
	{
	protected:
		T* mData;
		unsigned int mSize;
		unsigned int mMax;
	public:
		explicit Vector() :mSize(0), mMax(4)
		{
			mData = new T[mMax];
		}
		explicit Vector(size_t size) :mSize(size), mMax(size)
		{
			mData = new T[size];
		}
        Vector(const T* rawData,size_t size):mSize(size),mMax(size)
        {
            mData = new T[size];
            for(ssize_t i = 0;i<size;i++)
            {
                mData[i] = rawData[i];
            }
        }
		~Vector()
		{
			if (mData != nullptr)
			{
				delete[] mData;
				mData = nullptr;
			}
		}
		virtual void PushBack(const T& other)
		{
			if (IsFull())
			{
				Resize(mSize * 2);
			}
			mData[mSize] = other;
			mSize++;
		}
		virtual T& operator[](unsigned int index)
		{
			Check(index, " operator[]");
			return (mData[index]);
		}
		virtual T At(unsigned int index)
		{
			Check(index, " operator[]");
			return (mData[index]);
		}
		void Resize(unsigned int nSize)
		{
			T* newArr = nullptr;
			if (nSize > 0)
			{
				newArr = new T[nSize];
			}

			for (int i = 0; i < mSize; i++)
			{
				if (i < nSize)
				{
					newArr[i] = mData[i];
				}
			}

			if (mData != nullptr)
			{
				delete[]mData;
				mData = nullptr;
			}
			if (mSize > nSize)
			{
				mSize = nSize;
			}
			mData = newArr;
			mMax = nSize;
		}
		virtual inline unsigned int Size() const
		{
			return mSize;
		}
		virtual int IndexOf(const T& obj) const
		{
			for (int i = 0; i < mSize; i++)
			{
				if (mData[i] == obj)
				{
					return i;
				}
			}
			return -1;
		}
		virtual void Clear()
		{
			Resize(0);
		}
		virtual void RemoveAt(unsigned int index)
		{
			if (index >= mSize)
			{
				return;
			}

			mSize--;
			for (int i = index; i < mSize; i++)
			{
				mData[i] = mData[i + 1];
			}
			if (mSize < mMax / 4)
			{
				Resize(mMax / 4);
			}
		}
		virtual void Remove(const T&obj)
		{
			int index = IndexOf(obj);
			if (index != -1)
			{
				RemoveAt(index);
			}
		}
		void RemoveAll(const T&obj)
		{
			Vector<size_t> indexlist;
			for (size_t i = 0; i < Size(); i++)
			{
				if (At(i) != obj)
				{
					indexlist.PushBack(i);
				}
			}
			Vector<T> newVector(indexlist.Size());
			for (size_t i = 0; i < indexlist.Size(); i++)
			{
				newVector[i] = At(indexlist[i]);
			}
			auto data = mData;
			mData = newVector.data();
			newVector.mData = data;
		}
		bool IsFull() const
		{
			return mSize == mMax;
		}
		virtual bool Empty() const
		{
			return mSize <= 0;
		}
		T* data()
		{
			return mData;
		}
        const T* data() const
        {
            return mData;
        }
	private:
		void Check(unsigned index, const std::string& str) const
		{
			if (index >= mSize)
			{
				throw std::out_of_range(str);
			}
		}
	};

	static void VectorMain()
	{
		Vector<int> mArr;
		for (int i = 0; i < 100; i++)
		{
			mArr.PushBack(i);
		}
		mArr.Resize(50);
		mArr.RemoveAt(6);
		std::cout << "Size:" << mArr.Size() << std::endl;
		mArr.Remove(20);
		for (int i = 0; i < mArr.Size(); i++)
		{
			std::cout << mArr[i] << std::endl;
		}
	}
}

#endif
#endif