
#include <iostream>
using namespace std;

//���캯���е����麯���ܷ�����̬��?

class Parent301
{
public:
	Parent301(int a = 0)
	{
		this->a = a;
		print();        //���ã����ǵ� ��Ϊ��ʱ���ָ�뻹δ��ʼ��
	}

	virtual void print()  
	{
		cout<<"���ǵ�"<<endl;
	}
	
private:
	int a;
};

class Child301 : public Parent301
{
public:
	Child301(int a = 0, int b = 0) :Parent301(a)
	{
		this->b = b;
		print();
	}

	virtual void print()
	{
		cout<<"���Ƕ���"<<endl;
	}
private:
	int b;
};

void main301()
{
	Child301  c1; //����һ��������� ,�����������,�ڸ��๹�캯���е����麯��print �ܷ�����̬��?
	
	//c1.print();
}