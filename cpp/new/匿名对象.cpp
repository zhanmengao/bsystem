#include<iostream>;
using namespace std;

class ABCD {
public:
	ABCD(int a, int b)
	{
		cout << "构造" << endl;
	}
	~ABCD()
	{
		cout << "析构" << endl;

	}
	ABCD(ABCD &obj)
	{
		cout << "拷贝构造" << endl;
	}
private:
	int a;
	int b;
};
ABCD pppp()
{
	ABCD a(1, 2);
	return a;
}
int main()
{
	ABCD a1 = pppp();				//拷贝构造一个返回值给到a1
	system("pause");
	return 0;
}