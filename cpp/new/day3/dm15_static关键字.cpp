
#include <iostream>
using namespace std;

class BB
{
public:
	void printC()
	{
		cout<<"c = "<<c<<endl;
	}
	void AddC()
	{
		c = c + 1;
	}
	static void getC() //��̬��Ա���� 
	{
		cout<<"c:"<<c<<endl;
		//���ھ�̬��Ա������,�ܵ��� ��ͨ��Ա����  ���� ��ͨ��Ա������?
		//cout<<"a:"<<a<<endl; //error C2597: �ԷǾ�̬��Ա��BB::a���ķǷ�����
	}
protected:
private:
	int a; 
	int b;
	static int c; //��̬��Ա����
};

//��̬������ ����ʹ�� ��ͨ��Ա���� ��ͨ��Ա���� ..
int BB::c = 10;

void main1501()
{
	
	BB b1, b2, b3;
	b1.printC(); //10
	b2.AddC(); //11
	b3.printC(); //11
	

	//��̬��Ա�����ĵ��÷���
	b3.getC(); //�ö���.
	BB::getC();//��::

}