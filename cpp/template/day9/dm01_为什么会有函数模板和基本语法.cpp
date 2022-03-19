
#include <iostream>
using namespace std;

namespace demo1
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


	//�� ���Ͳ����� ===, �������Ա���б���
	// ���ͱ�� 
	//template ����C++������ ��Ҫ��ʼ���ͱ���� .����T, ��Ҫ��㱨��
	template <typename T>
	void myswap(T &a, T &b)
	{
		T c = 0;
		c = a;
		a = b;
		b = c;
		cout << "hello ....����ģ�庯�� ��ӭ calll ��" << endl;
	}


	//����ģ��ĵ���
	// ��ʾ���� ����
	// �Զ����� �Ƶ�
	void main101()
	{
		{

			int x = 10;
			int y = 20;

			myswap<int>(x, y); //1 ����ģ�� ��ʾ���� ����

			myswap(x, y);  //2 �Զ����� �Ƶ�
			printf("x:%d y:%d \n", x, y);
		}


	{

		char a = 'a';
		char b = 'b';

		//myswap<char>(a, b); //1 ����ģ�� ��ʾ���� ����

		myswap(a, b);
		printf("a:%c b:%c \n", a, b);
	}
	}
}
using namespace demo1;
void main102()
{
	{
		int x = 10;
		int y = 20;

		myswap01(x, y);
		printf("x:%d y:%d \n", x, y);

	}

	{
		char a = 'a';
		char b = 'b';

		myswap02(a, b);
		printf("a:%c b:%c \n", a, b);
	};
}

