
#include <iostream>
using namespace std;

void objplaymain71();

class Test7
{
public:
	Test7()
	{
		a = 0;
		b = 0;
		cout << "�޲������캯��  �Զ�������" << endl;
	}
	explicit Test7(int _a) //�в������캯��
	{
		a = _a;
		b = 0;
	}

	Test7(const Test7& obj) //copy���캯�� ����: ��һ�������ʼ������һ������
	{
		a = obj.a + 100;
		b = obj.b + 100;
	}
	void printT()
	{
		cout << "a:" << a << "b: " << b << endl;
	}

	~Test7()
	{
		cout << "������������ �����������ڽ���ʱ,�ᱻc++�������Զ�����" << endl;
	}

protected:
private:
	int a;
	int b;
};

// ��3�ֵ���ʱ��
void printTest(Test7 t)
{
	;
}

// 1 �� 2 
void objplaymain72()
{
	//
	Test7 t1(1); //ok
	Test7 t2(t1);
	Test7 t3 = t1; //�����copy���캯��
	printTest(t3);
}


//copy���캯���ĵ�4�ֵ���ʱ��
//����һ��Ԫ��  ��������
Test7 getTestObj()
{
	Test7 t(1);
	return t;
}

void TestNoNameObj()
{
	Test7  myt1 = getTestObj(); //�����������ʼ�� ����һ������  
	Test7  myt2(1);
	myt2 = getTestObj(); //���������� �� ����һ������ ��ֵ ������������

}

void main701()
{
	//objplaymain();
	objplaymain72();
	TestNoNameObj();
	cout << "hello..." << endl;
	system("pause");
}

void objplaymain71()
{
	Test7 t1; //ok
	//Test t2() ; //�����޲������캯���� ���󷽷�
	//t2.printT();

	//

	Test7  t3(1);	//c++�������Զ��ĵ��ù��캯��
	//Test t4 = 4;	//c++�������Զ��ĵ��ù��캯�����ᱻexplicit��ֹ��
	Test7 t5 = Test7(5); //����Ա�ֹ��ĵ��ù��캯��

	//
	Test7 t6 = t1;
	return;
}
