
#include <iostream>
using namespace std;

class Test4
{
public:
	Test4()  //�޲������캯��
	{
		m_a = 0;
		m_b = 0;
		cout << "�޲������캯��" << endl;
	}

	Test4(int a)
	{
		m_a = a;
		m_b = 0;
	}

	Test4(int a, int b) //�в������캯��   //3�ַ���
	{
		m_a = a;
		m_b = b;
		cout << "�в������캯��" << endl;
	}

	//��ֵ���캯�� (copy���캯��) //
	/*explicit*/ Test4(const Test4& obj)
	{
		cout << "��Ҳ�ǹ��캯�� " << endl;
		m_b = obj.m_b + 100;
		m_a = obj.m_a + 100;
	}

public:
	void printT()
	{
		cout << "��ͨ��Ա����" << endl;
		cout << "m_a" << m_a << " m_a" << m_b << endl;
	}
private:
	int m_a;
	int m_b;
};

//1  ��ֵ���캯�� ��1������ȥ��ʼ������һ������  
void main401()
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);

	//��ֵ=���� �᲻����ù��캯��
	//operator=()//��ש
	t0 = t1; //��t1 �� t0��ֵ  ������ �� ��ʼ����������ͬ�ĸ���

	//��1�ֵ��÷���
	Test4 t2 = t1; //��t1����ʼ�� t2   ��explicit�ؼ������������ֵ��ûᱨ��
	t2.printT();

	cout << "hello..." << endl;
	system("pause");
	return;
}

//�ڶ��ֵ���ʱ��
void main402()
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);

	Test4 t2(t1);  //��t1���� ��ʼ�� t2���� 
	t2.printT();

	cout << "hello..." << endl;
	system("pause");
	return;
}