
#include <iostream>
using namespace std;

/*

����ģ����ƽ���
�����������ǰѺ���ģ�崦����ܹ�����������ĺ���
�������Ӻ���ģ��ͨ���������Ͳ�����ͬ�ĺ���
��������Ժ���ģ��������α���
�������ĵط���ģ����뱾����б��룻�ڵ��õĵط��Բ����滻��Ĵ�����б��롣
*/
namespace demo05
{
	// ������ҵ���߼� һ��
	// �����Ĳ������� ��һ��
	void myswap01(int &a, int &b)
	{
		int c = 0;
		c = a;
		a = b;
		b = c;
	}

	void myswap02(char &a, char &b)
	{
		char c = 0;
		c = a;
		a = b;
		b = c;
	}


	// 1.cpp

	// g++ -S 1.cpp  -o 1.s
	template <typename T>
	void myswap(T &a, T &b)
	{
		T c = NULL;
		c = a;
		a = b;
		b = c;
		cout << "hello ....����ģ�庯�� ��ӭ calll ��" << endl;
	}
}

using namespace demo05;
void main501()
{
	{
		int x = 10;
		int y = 20;

		myswap<int>(x, y); //1 ����ģ�� ��ʾ���� ����

		printf("x:%d y:%d \n", x, y);
	}


	{
		char a = 'a';
		char b = 'b';

		myswap<char>(a, b); //1 ����ģ�� ��ʾ���� ����
		printf("a:%c b:%c \n", a, b);
	}
}
