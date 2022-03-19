/*
�����ģ����̳е�ʱ��,��Ҫ�ñ�����֪�� ������������;�����ʲô
(�������͵ı���:�̶���С�ڴ��ı���)A<int>
*/
#include <iostream>
using namespace std;

//A���ģ�� �� 
//ģ����  ���Ͳ�����


//��ģ��Ķ���
//��ģ���ʹ��
//��ģ�� ����������
namespace demo06
{
	//ģ���� 
	template <class T>
	class A
	{
	public:
		A(T a)
		{
			this->a = a;
		}
	public:
		void printA()
		{
			cout << "a: " << a << endl;
		}
	protected:
		T a;
	};

	//��ģ���� ������ ��ͨ��
	// ģ��������ʱ, ��Ҫ���廯ģ����. C++��������Ҫ֪�� ������������;�����ʲô���ӵ�
	//=====> Ҫ֪��������ռ���ڴ��С�Ƕ��� ֻ���������͹̶�����,��֪����η����ڴ� 
	class B : public A < int >
	{
	public:
		B(int a = 10, int b = 20) : A<int>(a)
		{
			this->b = b;
		}
		void printB()
		{
			cout << "a:" << a << " b: " << b << endl;
		}

	protected:
	private:
		int b;
	};


	//��ģ���� ���� ģ����
	template <typename T>
	class C : public A < T >
	{
	public:
		C(T c, T a) : A<T>(a)
		{
			this->c = c;
		}
		void printC()
		{
			cout << "c:" << c << endl;
		}
	protected:
		T c;
	};
	//��ģ�� ����������


	//���� ,C++������ Ҫ�������� ������ Ҫ A<int> &a 
	void UseA(A<int> &a)
	{
		a.printA();
	}
}

using namespace demo06;
void main601()
{
	C<int> c1(1, 2);
	c1.printC();
}

//
void main602()
{
	B  b1(1, 2);
	b1.printB();
}

void main603()
{
	//ģ����(����������ͻ���)====�������=====>�������ı���

	A<int> a1(11), a2(20), a3(30); //ģ�����ǳ����  ====>��Ҫ���� ���;���
	//a1.printA();

	UseA(a1);
	UseA(a2);
	UseA(a3);
}