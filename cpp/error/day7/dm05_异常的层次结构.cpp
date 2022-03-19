
#include <iostream>
using namespace std;

class MyArray
{
public:
	MyArray(int len);
	~MyArray();
public:
	int & operator[](int index);
	int getLen();
	//�ڲ���
	class eSize
	{
	public:
		eSize(int size)
		{
			m_size = size;
		}
		virtual void printErr()
		{
			cout << "size:" << m_size << " ";
		}
	protected:
		int m_size;
	};

	class eNegative : public eSize
	{
	public:
		eNegative(int size) : eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eNegative ���� size:" << m_size << " ";
		}
	};

	class eZero : public eSize
	{
	public:
		eZero(int size) : eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eZero ���� size:" << m_size << " ";
		}
	};

	class eTooBig : public eSize
	{
	public:
		eTooBig(int size) : eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eTooBig ���� size:" << m_size << " ";
		}
	};

	class eTooSmall : public eSize
	{
	public:
		eTooSmall(int size) : eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eTooSmall ���� size:" << m_size << " ";
		}
	};

private:
	int *m_space;
	int m_len;
};


MyArray::MyArray(int len)
{
	if (len < 0)
	{
		throw eNegative(len);   //�Զ����쳣��
	}
	else if (len == 0)
	{
		throw eZero(len);
	}
	else if (len > 1000)
	{
		throw eTooBig(len);
	}
	else if (len < 3)
	{
		throw eTooSmall(len);
	}
	m_len = len;
	m_space = new int[len];
}

MyArray::~MyArray()
{
	if (m_space != NULL)
	{
		delete[] m_space;
		m_space = NULL;
		m_len = 0;
	}
}

int & MyArray::operator[](int index)
{
	return m_space[index];
}

int MyArray::getLen()
{
	return m_len;
}

void main501()
{

	try
	{
		MyArray a(-5);
		for (int i = 0; i < a.getLen(); i++)
		{
			a[i] = i + 1;
			printf("%d ", a[i]);
		}
	}
	catch (MyArray::eSize &e)
	{
		//cout <<  "len�Ĵ�С: " << e.eSize();
		e.printErr();
	}
	catch (...)
	{
	}

}

void main502()
{

	try
	{
		MyArray a(-5);
		for (int i = 0; i < a.getLen(); i++)
		{
			a[i] = i + 1;
			printf("%d ", a[i]);
		}
	}
	catch (MyArray::eNegative e)
	{
		cout << "eNegative �����쳣" << endl;
	}
	catch (MyArray::eZero e)
	{
		cout << "eZero �����쳣" << endl;
	}
	catch (MyArray::eTooBig e)
	{
		cout << "eTooBig �����쳣" << endl;
	}
	catch (MyArray::eTooSmall e)
	{
		cout << "eTooSmall �����쳣" << endl;
	}

	catch (...)
	{
	}


}