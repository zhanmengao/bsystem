
#include <iostream>
using namespace std;
#include "MyVector.h"

template <typename T>
ostream& operator<<(ostream& out, const MyVector<T> &obj)
{
	for (int i = 0; i < obj.m_len; i++)
	{
		out << obj.m_space[i] << " ";
		//out << t1;
	}
	out << endl;
	return out;
}

//MyVector<int> myv1(10);
template <typename T>
MyVector<T>::MyVector(int size)  //���캯��
{
	m_space = new T[size];
	m_len = size;
}


//MyVector<int> myv2  = myv1;
template <typename T>
MyVector<T>::MyVector(const MyVector &obj) // �������캯��
{
	//����myv1�Ĵ�С�����ڴ�
	m_len = obj.m_len;
	m_space = new T[m_len];

	//copy����
	for (int i=0; i<m_len; i++)
	{
		m_space[i] = obj.m_space[i];
	}

}

template <typename T>
MyVector<T>::~MyVector() //��������
{
	if (m_space != NULL)
	{
		delete [] m_space;
		m_space = NULL;
		m_len = 0;
	}
}

template <typename T>
T& MyVector<T>::operator[] (int index)
{
	return m_space[index];
}


// a3 = a2 = a1;
template <typename T>
MyVector<T> &  MyVector<T>::operator=(const MyVector<T> &obj)
{
	//�Ȱ�a2�ľɵ��ڴ��ͷŵ�

	if (m_space != NULL)
	{
		delete[] m_space;
		m_space = NULL;
		m_len = 0;
	}

	//����a1�����ڴ� 
	m_len = obj.m_len;
	m_space = new T[m_len];

	//copy����
	for (int i=0; i<m_len; i++)
	{
		m_space[i] = obj[i];
	}
	return *this;  // a2 = a1; ���ظ�a2 ������
}
