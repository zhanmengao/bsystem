
#include <iostream>
using namespace std;

class F
{
public:
	int operator() (int a, int b)
	{
		return a*a + b*b;
	}
};

class F2
{
public:
	//��Ա������ʽ
	int MemFunc(int a, int b)
	{
		return a*a + b*b;
	}
};

//��������
class F3
{
public:
	int operator() (int a, int b)
	{
		c = a*a + b*b + c;
		return c;
	}
private:
	int c = 9;
};

void main606()
{
	
	F f;
	cout << f(2, 4) << endl;

	F2 f2;
	cout << f2.MemFunc(2, 4) << endl;

	F3 f3;
	cout << f3(2, 4) << endl;
	cout << f3(2, 4) << endl;
	return ;
}