
#include <iostream>
using namespace std;

class Test
{
public:
	int a;
	int b;

public:
	~Test()
	{
		cout<<"a:"<<a<<" b: "<<b;
		cout<<"���������Զ�������"<<endl;
	}

public:
	void printT()
	{
		cout<<"a:"<<a<<" b: "<<b<<endl;
	}

public:
	//t3 = t1.TestAdd(t2);
	Test TestAdd(Test &t2)
	{
		Test tmp(this->a + t2.a, this->b + t2.b);
		return tmp;
	}

	//t1.TestAdd2(t2);
	//����һ������ �൱�ڷ������� 
	//����t1���Ԫ�� this����&t1
	Test& TestAdd2(Test &t2)
	{
		this->a = this->a + t2.a;
		this->b = this->b + t2.b;
		
		return *this; //�� *(&t1) �ֻص��� t1Ԫ��
	}

public:
	Test(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
};

//�ѳ�Ա���� ת�� ȫ�ֺ��� ����һ������
void printT(Test *pT)
{
	cout<<"a:"<<pT->a<<" b: "<<pT->b<<endl;
}

//ȫ�ֺ����ķ��� 
//ȫ�ֺ��� ת�� ��Ա����  ����һ������ 
Test TestAdd(Test &t1, Test &t2)
{
	Test tmp;
	return tmp;
}

void main1801()
{
	Test t1(1, 2);
	Test t2(3, 4);

	//t1 = t1 + t2;
	t1.TestAdd2(t2);
	t1.printT();
}

void main1802()
{
	Test t1(1, 2);
	Test t2(3, 4);

	Test t3 ;

	//ȫ�ֺ�������
	t3 = TestAdd(t1, t2);
	//��Ա��������
	{
		//�ȰѲ��԰���д����
		Test t4 = t1.TestAdd(t2); //��������ֱ��ת����t4
		t4.printT();
		Test t5;
		t5 = t1.TestAdd(t2); //�������� ���� ��t5
		t5.printT();
	}


	cout<<"hello..."<<endl;
	system("pause");
	return ;
}