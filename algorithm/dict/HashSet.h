#pragma once
#include"Hash.h"
#include<stdint.h>
#include"../stb/List.hpp"
#include"../stb/array.hpp"

namespace stb
{
	//������������HashSet
	template<typename T>
	class HashSet
	{
	public:
		HashSet()
		{
			mArray.ResetArray(997);
		}
		HashSet(uint32_t hashLen)
		{
			mArray.ResetArray(hashLen);
		}
		//���뵽hashֵ��Ӧ������β��
		void Insert(const T& val)
		{
			size_t hash = Hash(val);
			auto& list = mArray[hash];
			list.PushBack(val);
		}
		//ͳ�Ƹ������ҵ�hashֵ�������ж�Ӧ����������������
		size_t Count(const T& val)
		{
			size_t count = 0;
			size_t hash = Hash(val);
			List<T>& list = mArray[hash];
			auto pNode = list.Begin();
			if (pNode != nullptr)
			{
				do
				{
					if (pNode->Data == val)
					{
						count++;
					}
					pNode = pNode->pRight;
				} while (pNode != mList.Begin());
			}
		}
		void print()
		{
			for (size_t i = 0; i < mArray.Length(); i++)
			{
				List<T>& list = mArray[i];
				auto pNode = list.Begin();
				if (pNode != nullptr)
				{
					do
					{
						std::cout << pNode->Data << "	";
						pNode = pNode->pRight;
					} while (pNode != list.Begin());
					std::cout << std::endl;
				}
			}
		}
	private:
		size_t Hash(const T& val)
		{
			if (mArray.Length() == 0)
			{
				return -1;
			}
			return GetHashCode<T>(val) % mArray.Length();			//Code��������ȡ��
		}
	private:
		Array<List<T>> mArray;				//
	};
}
