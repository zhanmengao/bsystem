
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;


//虚析构函数
class A
{
public:
	A()
	{
		p = new char[20];
		strcpy(p, "obja");
		printf("A()\n");
	}
	 virtual ~A()
	{
		delete [] p;
		printf("~A()\n");
	}
protected:
private:
	char *p;
};

class B : public A
{
public:
	B()
	{
		p = new char[20];
		strcpy(p, "objb");
		printf("B()\n");
	}
	  ~B()
	{
		delete [] p;
		printf("~B()\n");
	}
protected:
private:
	char *p;
};


class C : public B
{
public:
	C()
	{
		p = new char[20];
		strcpy(p, "objc");
		printf("C()\n");
	}
	~C()
	{
		delete [] p;
		printf("~C()\n");
	}
protected:
private:
	char *p;
};



//只执行了 父类的析构函数
//向通过父类指针  把 所有的子类对象的析构函数 都执行一遍
//向通过父类指针 释放所有的子类资源 
void howtodelete(A *base)
{
	delete base;  //这句话不会表现成多态 这种属性
}

/*
void howtodelete(B *base)
{
	delete base;  //这句话不会表现成多态 这种属性
}
*/
void main1501()
{
	C *myC = new C; //new delete匹配
	//
	delete myC; //直接通过子类对象释放资源 不需要写virtual 

	//howtodelete(myC);
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}