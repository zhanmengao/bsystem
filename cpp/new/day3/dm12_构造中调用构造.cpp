#include "iostream"
using namespace std;


//�����е��ù�����Σ�յ���Ϊ
class MyTest
{
public:
	MyTest(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}

	MyTest(int a, int b)
	{
		this->a = a;
		this->b = b;

		MyTest(a, b, 100); //�����µ���������
	}
	~MyTest()
	{
		printf("OnDestroy,MyTest~:%d, %d, %d\n", a, b, c);
	}

protected:
private:
	int a;
	int b;
	int c;

public:
	int getC() const { return c; }
	void setC(int val) { c = val; }
};

int main1201()
{
	MyTest t1(1, 2);
	printf("c:%d \n", t1.getC()); //����c��ֵ�ǣ� δ��ֵ�����
	return 0;
}