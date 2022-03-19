//C++11 shared_ptr 智能指针 的使用,避免内存泄露
#include <iostream>
#include <memory>
#include<string.h>

using namespace std;
#ifdef __linux
#define strcpy_s(b,l,s) strncpy(b,s,l)
#endif
#define _PRT(T) std::shared_ptr<T>

//定义 shared_ptr<T> 的智能指针
#ifdef __WIN32
#define _PRTO(T,N,...)  std::shared_ptr<T> N(new T(##__VA_ARGS__))
#else
#define _PRTO(T,N,V)  std::shared_ptr<T> N(new T(V))
#endif

//定义 shared_ptr<T> 的数组智能指针
#define _PRTA(T,N,n)    std::shared_ptr<T> N(new T[n])

class A
{
public:
	int n;
    A(int n) :n(n)
    {
		cout << n << " construct A!!!" << endl;
	}

    ~A()
    {
		cout << n << " destruct A!!!" << endl;
	}

	void Out() { cout << n * 2 << endl; }
};
class B : public A
{
public:
    B(int n) :A(n)
    {
		cout << n << " construct B!!!" << endl;
	}

    ~B()
    {
		cout << n << " destruct B!!!" << endl;
	}


	_PRT(A) geta(int n) { _PRTO(A, a, n); return a; }


	void chars()
	{
		//使用智能指针指向 char[],以自动释放
		_PRTA(char, p, 1024 * 1024);
		strcpy_s(p.get(),1024*1024,"std::shared_ptr<char*>");
		printf(p.get());
	}

};
void mainChar()
{
	B* ptrB0 = new B(1);
	ptrB0->Out();

	_PRT(B) ptrB1(new B(2));
	ptrB1->Out();
	_PRT(A) a = ptrB1->geta(5);
	a->Out();
	//复制了指针,增加引用计数
	_PRT(B) ptrB2 = ptrB1; 
	_PRT(A) b = ptrB2->geta(6);
	b->Out();
	ptrB2->Out();

	//使用智能指针,会自动释放
	for (int i = 100; i; i--)
		ptrB2->chars();
}
