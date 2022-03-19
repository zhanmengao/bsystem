#pragma  once
#include <iostream>
using namespace std;

namespace stb
{
	template<class T>
	class Array
	{
	public:
		Array()
		{
			m_length = 0;
			m_space = nullptr;
		}
		Array(unsigned int length)
		{
			ResetArray(length);
		}
		Array(const Array<T>& obj)
		{
			this->m_length = obj.m_length;
			this->m_space = new T[this->m_length]; //分配内存空间
			 //数组元素复制
			memmove(m_space, obj.m_space, sizeof(T)*m_length);
			//for (int i = 0; i < m_length; i++)
			//{
			//	this->m_space[i] = obj.m_space[i];
			//}
		}
		~Array()
		{
			if (m_space != NULL)
			{
				delete[] m_space;
				m_space = NULL;
				m_length = 0;
			}
		}
	public:
		void ResetArray(unsigned int length)
		{
			Release();
			m_length = length;
			m_space = new T[m_length];
		}
		inline unsigned int Length() const
		{
			return m_length;
		}

	private:
		unsigned int m_length;
		T *m_space;

	public:
		//函数返回值当左值，需要返回一个引用
		//应该返回一个引用(元素本身) 而不是一个值
		T& operator[](unsigned int index)
		{
			Check(index, "operator[] error!");
			return m_space[index];
		}
		T operator[](unsigned index) const
		{
			Check(index, "operator[] error!");
			return m_space[index];
		}
		//重载=
		Array& operator=(Array &a1)
		{
			//1 释放原来的内存空间
			Release();
			//2 根据a1大小 分配内存 
			m_length = a1.m_length;
			m_space = new int[m_length];

			//3 copy数据
			memmove(a1.m_space, m_space, sizeof(T)* m_length);
			//for (unsigned int i = 0; i < m_length; i++)
			//{
			//	m_space[i] = a1[i];
			//}

			return *this;
		}
		//重载 ==
		bool operator==(const Array &a1)
		{
			if (this->m_length != a1.m_length)
			{
				return false;
			}
			for (int i = 0; i < m_length; i++)
			{
				if (this->m_space[i] != a1[i])
				{
					return false;
				}
			}
			return true;
		}
		//重载 !=
		bool operator!=(Array &a1)
		{
			return !(*this == a1);
		}
	private:
		void Check(unsigned index, const std::string& str) const
		{
			if (index >= Length())
			{
				throw std::out_of_range(str);
			}
		}
		void Release()
		{
			if (this->m_space != NULL)
			{
				delete[] m_space;
				m_length = 0;
			}
		}
	};
}