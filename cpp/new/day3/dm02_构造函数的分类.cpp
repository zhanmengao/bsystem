

#include <iostream>
using namespace std;


class Test2
{
public:
	Test2()  //�޲������캯��
	{
		m_a = 0;
		m_b = 0;
		cout<<"�޲������캯��"<<endl;
	}
	
	Test2(int a)
	{
		m_a = a;
		m_b = 0;
	}

	Test2(int a, int b) //�в������캯��   //3�ַ���
	{
		m_a = a;
		m_b = b;
		cout<<"�в������캯��"<<endl;
	}

	//��ֵ���캯�� (copy���캯��) //explicit�ؼ��ֽ���ֹ������챻��ʽ����
	Test2(const Test2& obj )
	{
		cout<<"��Ҳ�ǹ��캯�� " <<endl;
	}

public:
	void printT()
	{
		cout<<"��ͨ��Ա����"<<endl;
	}
private:
	int m_a;
	int m_b;
};

void main201()
{
	
	Test2 t1;  //�����޲������캯��
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//���� �����в������캯�� 3
void main202()
{
	//1���ŷ� 
	Test2 t1(1, 2);  //���ò������캯��  c++�������Զ��ĵ��ù��캯��
	t1.printT();

	// 2 =�ŷ�
	Test2 t2 = (3, 4, 5, 6, 7); // = c+�ԵȺŷ� ������ǿ  c++�������Զ��ĵ��ù��캯�� 

	Test2 t3 = 5;

	//3 ֱ�ӵ��ù��캯��  �ֶ��ĵ��ù��캯��
	Test2 t4 = Test2(1, 2);   //�������� (���������ȥ����) ��ש ....//t4����ĳ�ʼ��

	t1 =  t4;  //��t4 copy�� t1  //��ֵ���� 
	//����ĳ�ʼ�� �� ����ĸ�ֵ ��������ͬ�ĸ��� 
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}