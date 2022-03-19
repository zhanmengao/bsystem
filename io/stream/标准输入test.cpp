
#include <iostream>
using namespace std;
#include "string"

/*
��׼����������cin
	cin.get() //һ��ֻ�ܶ�ȡһ���ַ�
	cin.get(һ������) //��һ���ַ�
	cin.get(��������) //���Զ��ַ���
	cin.getline()    //һ�ζ�һ��
	cin.ignore()     //����size���ֽ�
	cin.peek()      //��size���ֽڣ����������ݴӻ�����ȡ����
	cin.putback()   
*/


void main701()
{
	char	mybuf[1024];
	int		myInt;
	long	myLong;

	cin >> myInt;

	cin >> myLong;

	cin >> mybuf; // �����ո�ֹͣ���� ���� 

	cout << "myInt:" << myInt << "myLong" << myLong << "mybuf:" << mybuf << endl;

}

//get()
void main702()
{
	char ch;
	while ( (ch=cin.get() )!= EOF )
	{
		cout << ch << endl;
	}
}

void main703()
{
	char a, b, c;

	cout << "cin.get(a) ���������û������,��������� \n";
	cin.get(a);
	cin.get(b);
	cin.get(c);

	cout << a << b << c << "��Ϊ������������,���򲻻�����\n";

	cin.get(a).get(b).get(c);

	cout << a << b << c;
}

//getline�������Խ��� �ո񣺶�һ��
void main704()
{
	char buf1[256];
	char buf2[256];

	cout << "������һ���ַ��� ���ж���ո� aa bb cc dd\n";

	cin >> buf1;

	cin.getline(buf2, 256);

	cout << "buf1:" << buf1 << "buf2:" << buf2 << endl; 
}

void main705()
{
	char buf1[256];
	char buf2[256];

	cout << "������һ���ַ��� ���ж���ո�aa  bbccdd\n";

	cin >> buf1;
	cin.ignore(20);
	int myint = cin.peek();
	cout << "myint:" << myint << endl; 

	cin.getline(buf2, 256);

	cout << "buf1:" << buf1 << "\nbuf2:" << buf2 << endl; 
}

//����:������������ַ����ֿ�����
int main706() 
{
	while (1)
	{
		cout << "Please, enter a number or a word: ";
		char c = std::cin.get();

		if ((c >= '0') && (c <= '9')) //������������ַ��� �ֿ�����
		{
			int n; //�����м䲻�����пո� ����ʹ��cin >>n
			cin.putback(c);
			cin >> n;
			cout << "You entered a number: " << n << '\n';
		}
		else
		{
			string str;
			cin.putback(c);    //��cд�ص�����
			//cin.getline(str);
			getline(cin, str); // �ַ��� �м�����пո� ʹ�� cin.getline();
			cout << "You entered a word: " << str << '\n';
		}
	}
	return 0;
}
