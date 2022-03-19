//C++11 shared_ptr ����ָ�� ��ʹ��,�����ڴ�й¶
#include <iostream>
#include <memory>
#include<string.h>

using namespace std;
#ifdef __linux
#define strcpy_s(b,l,s) strncpy(b,s,l)
#endif
#define _PRT(T) std::shared_ptr<T>

//���� shared_ptr<T> ������ָ��
#ifdef __WIN32
#define _PRTO(T,N,...)  std::shared_ptr<T> N(new T(##__VA_ARGS__))
#else
#define _PRTO(T,N,V)  std::shared_ptr<T> N(new T(V))
#endif

//���� shared_ptr<T> ����������ָ��
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
		//ʹ������ָ��ָ�� char[],���Զ��ͷ�
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
	//������ָ��,�������ü���
	_PRT(B) ptrB2 = ptrB1; 
	_PRT(A) b = ptrB2->geta(6);
	b->Out();
	ptrB2->Out();

	//ʹ������ָ��,���Զ��ͷ�
	for (int i = 100; i; i--)
		ptrB2->chars();
}
