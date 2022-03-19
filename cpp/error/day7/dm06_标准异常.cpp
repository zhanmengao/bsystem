
// out_of_range
#include "iostream"
using namespace std;
#include <stdexcept> 
#include "string"

// out_of_range
class Teacher
{
public:
	Teacher(int age)
	{
		if (age > 100)
		{
			string  s = "年龄太大";
			throw out_of_range(s);
		}
		this->age = age;
	}
protected:
private:
	int age;
};

void main601()
{
	try
	{
		Teacher t1(102);
	}
	catch (out_of_range e)
	{

		cout << e.what() << endl;
	}

	exception e;          //异常基类
}

class MyException : public exception
{
public:
	MyException(const char *p)
	{
		this->m_p = p;
	}

	virtual const char *  what()
	{
		cout << "MyException: 类型" << m_p << endl;
		return m_p;
	}
protected:
private:
	const char *m_p;
};

void testMyExcept()
{
	throw MyException("函数异常");
}
void main602()
{

	try
	{
		testMyExcept();
	}
	catch (MyException & e)
	{
		e.what();
	}
	catch (...)
	{
		cout << "未知 类型 " << endl;
	}
	
}