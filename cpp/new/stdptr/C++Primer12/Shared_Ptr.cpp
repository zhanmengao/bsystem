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
int mainS1()
{
	shared_ptr<A> sp1 = shared_ptr<A>(new A(2));							//A(2)��sp1�йܣ�
	shared_ptr<A> sp2(sp1);									 //A(2)ͬʱ����sp2�й�
	shared_ptr<A> sp3;
	sp3 = sp2;												//A(2)ͬʱ����sp3�й�
	cout << sp1->i << "," << sp2->i << "," << sp3->i << endl;        //sp123ͬʱָ��2

	A* p = sp3.get();										// get�����йܵ�ָ�룬p ָ�� A(2)
	cout <<"sp3.get() = "<<p->i << endl;									//��� 2


	sp1.reset(new A(3));									// reset�����й��µ�ָ��, ��ʱsp1�й�A(3)
	sp2.reset(new A(4));									// sp2�й�A(4)      						
	sp3.reset(new A(5));									// sp3�й�A(5),A(2)�����йܣ���delete
	cout << sp1->i << "," << sp2->i << "," << sp3->i << endl;
	return 0;
}