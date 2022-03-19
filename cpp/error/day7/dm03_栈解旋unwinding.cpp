
#include <iostream>
using namespace std;

class Test3
{
public:
	Test3(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	~Test3()
	{
	}
private:
	int a;
	int b;
};

void myDivide() throw (int, char, char *)            //�ں������������throw ��ʾ������������׳���Щ���͵��쳣
{
	Test3 t1(1, 2), t2(3, 4);
	//throw Test3;
	throw 1;
}


//ֻ���� ���г�����
void myDivide1() throw (int, char, char *)
{
	Test3 t1(1, 2), t2(3, 4);
	//throw Test3;  //error��ֻ���׳�ָ�����͵��쳣
}

//��д ��throw(...),�����׳��κ�����
void myDivide2() 
{
	Test3 t1(1, 2), t2(3, 4);
	cout << "myDivide ...Ҫ�����쳣\n" ;
	//throw Test3;
	throw 1;
}
// ���׳��쳣
void myDivide3() throw ()
{
	Test3 t1(1, 2), t2(3, 4);
	cout << "myDivide ...Ҫ�����쳣\n" ;
	//throw Test3;
	throw 1;
}


void main301()
{
	try
	{
		myDivide();
	}
	catch (int a)
	{
		cout << "int���� �쳣\n" ;
	}

	catch (...)
	{
		cout << " δ֪ �쳣\n" ;
	}
}