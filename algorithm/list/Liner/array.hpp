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
			this->m_space = new T[this->m_length]; //�����ڴ�ռ�
			 //����Ԫ�ظ���
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
		//��������ֵ����ֵ����Ҫ����һ������
		//Ӧ�÷���һ������(Ԫ�ر���) ������һ��ֵ
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
		//����=
		Array& operator=(Array &a1)
		{
			//1 �ͷ�ԭ�����ڴ�ռ�
			Release();
			//2 ����a1��С �����ڴ� 
			m_length = a1.m_length;
			m_space = new int[m_length];

			//3 copy����
			memmove(a1.m_space, m_space, sizeof(T)* m_length);
			//for (unsigned int i = 0; i < m_length; i++)
			//{
			//	m_space[i] = a1[i];
			//}

			return *this;
		}
		//���� ==
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
		//���� !=
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