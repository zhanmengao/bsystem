extern void f1(int age);   //����Ҫʹ�õĺ���
extern void f2(int age);   //����Ҫʹ�õĺ���
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	f1(65);
	f2(66);
	cout << "bye" << endl;
	
	return 0;
}
