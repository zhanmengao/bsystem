#include<iostream>;
using namespace std;

class ABC{
public:
	ABC(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		cout << "ABC���캯��������"<<endl;
	}
	~ABC()
	{
		cout << "a=" << this->a << "b=" << this->b << "c=" << this->c;
		cout << "~ABC()��������������" << endl;
	}
private:
	int a;
	int b;
	int c;
};

class MyD{
public:
	MyD(int a, int b, int c, int da) :abc1(a, b, c), abc2(b, c, a), a(da)
	{
		cout << "MyD�Ĺ��캯��������" << endl;
	}
	~MyD()
	{
		cout << "MyD����������������" << endl;
	}
	int dothing(MyD myd1)
	{
		return 0;
	}
private:
	ABC abc1;
	ABC abc2;
	const int a;
	int b;

};

void objplayer()
{
	MyD myd(1,2,3,4);
	myd.dothing(myd);
}
int main11()
{
	objplayer();
	system("pause");
	return 0;
}