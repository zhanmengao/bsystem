#include<iostream>
#include <string>
using namespace std;

//operator>>  ��  operator<<
void main101()
{
	char mybuf[1024];
	int myInt;
	long myLong;
	cin >> myInt;
	cin >> myLong;
	
	cin >> mybuf;
	cout << "myInt" << myInt << "myLong"<<myLong<<
		"mybuf"<<mybuf<<endl;
	 
}

//cin.get ��ȡ��һ���ַ�c
void main102()
{ 
	char ch;
	while ((ch = cin.get()) != EOF)
	{
		cout<<ch<<endl;
	}
}

void main103()
{
	char a, b, c;
	cin.get(a).get(b).get(c);
	cout<<a<<b<<c<<endl;
}

//cin.getline(to,size) ��ȡһ���ַ�
void main104()
{
	char buf1[256];
	char buf2[256];
	cin >> buf1;
	cin.getline(buf2, 256);
	cout<<buf1<<endl;
	cout << buf2 << endl;
}

//cin.ignore(size) �����������е�20���ֽ�
//cin.peek():����һ��int ����������ֵ�ӻ�������ȡ����
void main105()
{
	char buf1[256];
	char buf2[256];
	cin >> buf1;
	
	cin.ignore(20);
	int a = cin.peek();
	cout << buf1 << endl;
	cout << a   << endl;
}

