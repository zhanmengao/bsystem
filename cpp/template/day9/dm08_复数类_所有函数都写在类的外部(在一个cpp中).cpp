
#include <iostream>
using namespace std;

namespace demo08
{


	template <typename T>
	class Complex; //���ǰ������

	template <typename T>
	Complex<T> MySub(Complex<T> &c1, Complex<T> &c2);


	template <typename T>
	class Complex
	{
		friend Complex<T> MySub<T>(Complex<T> &c1, Complex<T> &c2);

		friend ostream & operator<< <T> (ostream &out, Complex &c3);

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
	Complex<T>::Complex(T a, T b)
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
	Complex<T>  Complex<T>::operator+ (Complex<T> &c2)
	{
		Complex tmp(a + c2.a, b + c2.b);
		return tmp;
	}

	//��Ԫ���� ʵ�� << ���������

	/*
	1>dm08_������_���к�����д������ⲿ(��һ��cpp��).obj : error LNK2019: �޷��������ⲿ���� "
	class std::basic_ostream<char,struct std::char_traits<char> > &
	__cdecl operator<<
	(class std::basic_ostream<char,struct std::char_traits<char> > &,class Complex<int> &)"
	(??6@YAAAV?$basic_ostream@DU?$char_traits@D@std@@@std@@AAV01@AAV?$Complex@H@@@Z)��
	�÷����ں��� _main �б�����
	1>E:\01-work\21_CPlusPlus�γ�\day09\���ͱ�̿��ò���\Debug\���ͱ�̿��ò���.exe : fatal error LNK1120: 1 ���޷��������ⲿ����
	*/

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

using namespace demo08;
void main801()
{
	//��Ҫ��ģ���� ���о��廯�Ժ�  ���ܶ������  C++������Ҫ�����ڴ�
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//������Ԫ����
	{
		Complex<int> c4 = MySub<int>(c1, c2);
		cout << c4 << endl;

	}
}