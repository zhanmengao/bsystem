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
	auto_ptr<A> sp1(new A(2));							//A(2)由sp1托管，
	auto_ptr<A> sp2(sp1);									 //sp1转移到sp2，sp1为空
	auto_ptr<A> sp3;
	sp3 = sp2;												//sp2转移到sp3，sp2,sp1为空
	cout <<sp3->i << endl;

	A* p = sp3.get();										// get返回托管的指针，p 指向 A(2)
	cout << "sp3.get() = " << p->i << endl;									//输出 2

	sp3.reset(new A(5));									// reset导致托管新的指针,此时sp3托管A(5),A(2)无人托管，被delete
	cout << "sp3.reset(5)" << endl;
	sp1.reset(new A(3));									
	sp2.reset(new A(4));									// sp2托管A(4)      						\
	cout << sp1->i << "," << sp2->i << "," << sp3->i << endl;
	return 0;
}