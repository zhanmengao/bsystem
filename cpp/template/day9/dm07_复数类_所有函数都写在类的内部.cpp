
#include <iostream>
using namespace std;

namespace demo07
{
	template <typename T>
	class Complex
	{
		friend Complex MySub(Complex &c1, Complex &c2)
		{
			Complex tmp(c1.a - c2.a, c1.b - c2.b);
			return tmp;
		}

		friend ostream & operator<<(ostream &out, Complex &c3)
		{
			out << c3.a << " + " << c3.b << "i" << endl;
			return out;
		}
	public:
		Complex(T a, T b)
		{
			this->a = a;
			this->b = b;
		}

		Complex operator+ (Complex &c2)
		{
			Complex tmp(a + c2.a, b + c2.b);
			return tmp;
		}

		void printCom()
		{
			cout << "a:" << a << " b: " << b << endl;
		}
	private:
		T	a;
		T	b;
	};
	//��������ص�����д�� 
	// ���� << >> ֻ������Ԫ����  ,������������� ��Ҫд�ɳ�Ա���� , ��Ҫ������Ԫ����

	/*
	ostream & operator<<(ostream &out, Complex &c3)
	{
	out <<  "a:" << c3.a << " b: " << c3.b << endl;
	return out;
	}
	*/
}

using namespace demo07;
void main701()
{
	//��Ҫ��ģ���� ���о��廯�Ժ�  ���ܶ������  C++������Ҫ�����ڴ�
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//������Ԫ����
	{
		Complex<int> c4 = MySub(c1, c2);
		cout << c4 << endl;

	}
}