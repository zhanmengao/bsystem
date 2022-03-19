#include <iostream>
#include <memory>
using namespace std;


class A
{
public:
	int i;
	A(int n) :i(n) { };
	~A() { cout << i << " " << "destructed" << endl; }
};
int mainAuto()
{
	auto_ptr<A> sp1(new A(2));							//A(2)��sp1�йܣ�
	auto_ptr<A> sp2(sp1);									 //sp1ת�Ƶ�sp2��sp1Ϊ��
	auto_ptr<A> sp3;
	sp3 = sp2;												//sp2ת�Ƶ�sp3��sp2,sp1Ϊ��
	cout <<sp3->i << endl;

	A* p = sp3.get();										// get�����йܵ�ָ�룬p ָ�� A(2)
	cout << "sp3.get() = " << p->i << endl;									//��� 2

	sp3.reset(new A(5));									// reset�����й��µ�ָ��,��ʱsp3�й�A(5),A(2)�����йܣ���delete
	cout << "sp3.reset(5)" << endl;
	sp1.reset(new A(3));									
	sp2.reset(new A(4));									// sp2�й�A(4)      						\
	cout << sp1->i << "," << sp2->i << "," << sp3->i << endl;
	return 0;
}