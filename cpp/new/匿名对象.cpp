#include<iostream>;
using namespace std;

class ABCD {
public:
	ABCD(int a, int b)
	{
		cout << "����" << endl;
	}
	~ABCD()
	{
		cout << "����" << endl;

	}
	ABCD(ABCD &obj)
	{
		cout << "��������" << endl;
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
	ABCD a1 = pppp();				//��������һ������ֵ����a1
	system("pause");
	return 0;
}