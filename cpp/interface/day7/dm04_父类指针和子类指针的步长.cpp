
#include <iostream>
using namespace std;

//����:
//��̬���ø���ָ��ָ��������� �� ���ಽ��++,��������ͬ�ĸ���

class Parent401
{
public:
	Parent401(int a = 0)
	{
		this->a = a;
	}

	virtual void print()  
	{
		cout<<"���ǵ�"<<endl;
	}

private:
	int a;
};


//�ɹ� ,һ��żȻ�ĳɹ� ,��Ȼ��ʧ�ܸ�����
class Child401 : public Parent401
{
public:
	/*
	Child(int a = 0, int b=0):Parent(a)
	{
		this->b = b;
		print();
	}
	*/
	
	Child401(int b = 0) :Parent401(0)
	{
		//this->b = b;
	}
	

	virtual void print()
	{
		cout<<"���Ƕ���"<<endl;
	}
private:
	//int b;
};


void main()
{
	Parent401 *pP = NULL;
	Child401  *pC = NULL;

	Child401  array[] = { Child401(1), Child401(2), Child401(3) };
	pP = array;
	pC = array;

	pP->print();
	pC->print(); //��̬����


	pP++;                                   //��sizeof(Parent401)
	pC++;                                   //��sizeof(Child401)
	pP->print();
	pC->print(); 


	pP++;
	pC++;
	pP->print();
	pC->print(); 
}