#include<iostream>
#include <string>
using namespace std;

//operator>>  和  operator<<
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

//cin.get 获取下一个字符c
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

//cin.getline(to,size) 获取一行字符
void main104()
{
	char buf1[256];
	char buf2[256];
	cin >> buf1;
	cin.getline(buf2, 256);
	cout<<buf1<<endl;
	cout << buf2 << endl;
}

//cin.ignore(size) 丢弃输入流中的20个字节
//cin.peek():读下一个int 但不会把这个值从缓冲区中取出来
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

