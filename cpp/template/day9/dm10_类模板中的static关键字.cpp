
/*
ÿ����ͬ�ķ���ʵ ��static����ռ��ͬ�ڴ�
*/

#include <iostream>
using namespace std;

namespace demo10
{
	template <typename T>
	class AA
	{
	public:
		static T m_a;
	protected:
	private:
	};

	template <typename T>
	T AA<T>::m_a = 0;


	class AA1
	{
	public:
		static int m_a;
	protected:
	private:
	};
	int AA1::m_a = 0;


	class AA2
	{
	public:
		static char m_a;
	protected:
	private:
	};
	char AA2::m_a = 0;
}


using namespace demo10;
void main1001()
{
	AA<int> a1, a2, a3;
	a1.m_a = 10;
	a2.m_a++;
	a3.m_a++;
	cout << AA<int>::m_a << endl;

	AA<char> b1, b2, b3;
	b1.m_a = 'a';
	b2.m_a++;
	b2.m_a++;

	cout << AA<char>::m_a << endl;

	//m_a Ӧ���� ÿһ�����͵��� ʹ���Լ���m_a
}