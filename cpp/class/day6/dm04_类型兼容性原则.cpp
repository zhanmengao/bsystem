
#include <iostream>
using namespace std;

class Parent401
{
public:
	void printP()
	{
		cout<<"���ǵ�..."<<endl;
	}

	Parent401()
	{
		cout<<"parent���캯��"<<endl;
	}

	Parent401(const Parent401 &obj)
	{
		cout<<"copy���캯��"<<endl;
	}

private:
	int a;
};

class child401 : public Parent401
{
public:
	void printC()
	{
		cout<<"���Ƕ���"<<endl;
	}
protected:
private:
	int c;
};



/*
���ݹ�������ָ������������������������������
	���������Ե����������ʹ��
	����������ֱ�Ӹ�ֵ���������
	����������ֱ�ӳ�ʼ���������
	����ָ�����ֱ��ָ���������
	�������ÿ���ֱ�������������
	*/



//C++������ �ǲ��ᱨ��� .....
void howToPrint(Parent401 *base)
{
	base->printP(); //����� ��Ա���� 

}

void howToPrint2(Parent401 &base)
{
	base.printP(); //����� ��Ա���� 
}

void main401()
{
	//�����滻ԭ��

	Parent401 p1;
	p1.printP();

	child401 c1;
	c1.printC();
	c1.printP();


	//��ֵ������ԭ�� 
	//1-1 ����ָ�� (����) ָ�� �������
	Parent401 *p = NULL;
	p = &c1;
	p->printP();  

	//1-2 ָ������������

	howToPrint(&p1);
	howToPrint(&c1); 

	//1-3��������������
	howToPrint2(p1);
	howToPrint2(c1); 


	//�ڶ��㺬��

	//�������������   ��ʼ��   �������
	//�������һ������ĸ���
	Parent401 p3 = c1;

	
}