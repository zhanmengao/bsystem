
#include <iostream>
using namespace std;
namespace demo09
{
	template <typename T>
	class Complex
	{
		friend Complex<T> MySub(Complex<T> &c1, Complex<T> &c2);
		friend ostream & operator<<<T> (ostream &out, Complex<T> &c3);    //�ǵ��ں����������<T>

	public:
		Complex(T a, T b);
		void printCom();
		Complex operator+ (Complex &c2);

	private:
		T	a;
		T	b;
	};

	//���캯����ʵ�� д��������ⲿ
	template <typename T>
	Complex<T>::Complex<T>(T a, T b)
	{
		this->a = a;
		this->b = b;
	}

	template <typename T>
	void Complex<T>::printCom()
	{
		cout << "a:" << a << " b: " << b << endl;
	}

	//������ : ģ�������� �������ɵ� ��һ�����ɵĺ���ͷ �͵ڶ������ɵĺ���ͷ ��һ��
	//��Ա���� ʵ�� +���������
	template <typename T>
	Complex<T>  Complex<T>::operator+(Complex<T> &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}


	template <typename T>
	ostream & operator<<(ostream &out, Complex<T> &c3)
	{
		out << c3.a << " + " << c3.b << "i" << endl;
		return out;
	}

	//////////////////////////////////////////////////////////////////////////

	//���� ��Ԫ����
	template <typename T>
	Complex<T> MySub(Complex<T> &c1, Complex<T> &c2)
	{
		Complex<T> tmp(c1.a - c2.a, c1.b - c2.b);
		return tmp;
	}
}

using namespace demo09;
void main901()
{
	//��Ҫ��ģ���� ���о��廯�Ժ�  ���ܶ������  C++������Ҫ�����ڴ�
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//������Ԫ����
	{
		//Complex<int> c4 = MySub<int>(c1, c2);
		//cout << c4 << endl;

	}
}