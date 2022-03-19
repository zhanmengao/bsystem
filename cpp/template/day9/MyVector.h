#pragma once
#include <iostream>
using namespace std;

template <typename T>
class MyVector
{

	friend ostream& operator<<(ostream &out, const MyVector &obj)
	{
		for (int i = 0; i < obj.m_len; i++)
		{
			out << obj.m_space[i] << " ";
			//out << t1;
		}
		out << endl;
		return out;
	}
public:
	MyVector(int size = 0);  //构造函数
	MyVector(const MyVector &obj); // 拷贝构造函数
	~MyVector(); //析构函数

public:

	T& operator[] (int index);
	// a3 = a2 = a1;
	MyVector &operator=(const MyVector &obj);

	

public:
	int getLen()
	{
		return m_len;
	}

protected:
	T *m_space;
	int m_len;
};


