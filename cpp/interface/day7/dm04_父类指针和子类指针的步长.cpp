
#include <iostream>
using namespace std;

//结论:
//多态是用父类指针指向子类对象 和 父类步长++,是两个不同的概念

class Parent401
{
public:
	Parent401(int a = 0)
	{
		this->a = a;
	}

	virtual void print()  
	{
		cout<<"我是爹"<<endl;
	}

private:
	int a;
};


//成功 ,一次偶然的成功 ,必然的失败更可怕
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
		cout<<"我是儿子"<<endl;
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
	pC->print(); //多态发生


	pP++;                                   //走sizeof(Parent401)
	pC++;                                   //走sizeof(Child401)
	pP->print();
	pC->print(); 


	pP++;
	pC++;
	pP->print();
	pC->print(); 
}