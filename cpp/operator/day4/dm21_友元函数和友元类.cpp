
#include <iostream>
using namespace std;

class A
{
public:
	friend class B;//B�� �� A�ĺ����� ����B�п��Է���A���˽�г�Ա ˽�к���
	//1 ������λ�� �� public privateû�й�ϵ
	friend void modifyA(A *pA, int _a); //2 ����modifyA �� ��A�ĺ�����
	A(int a=0, int b=0)
	{
		this->a = a;
		this->b = b;
	}
	int getA()
	{
		return this->a;
	}

private:
	int a;
	int b;
};

//  

void modifyA(A *pA, int _a)
{
	//pA->a = 100;
	pA->a = _a;
}

//
class B
{
public:
	void Set(int a)
	{
		Aobject.a = a;
	}
	void printB()
	{
		cout<<Aobject.a <<endl;
	}
private:
	A Aobject;
};

//Ϊʲô�����Ԫ�ຯ��
// 1.java--->1.class���ֽ��룩 ==��������Ʒ���1.class �ҵ������ֱ���޸����˽�����ԡ�����
//������� ��Ϊһ�ֱ�׼��������jdk ...sun ���ɱ�׼ ������jdk �� api������������ 
//AOP
//2 1.cpp===>���
// Ԥ���� ����  ����  ���� ����gcc -E //gcc -s  -
//gcc -o 1.exe 1.c 
// ��������ҡ����������ѡ�������
//3 ����һ������ ������friend

/*
gcc -E hello.c -o hello.i��Ԥ����
gcc -S hello.i -o hello.s�����룩
gcc -c hello.s -o hello.o����ࣩ
gcc hello.o -o hello�����ӣ�
�����ĸ����裬�ɺϳ�һ������

gcc hello.c -o hello��ֱ�ӱ������ӳɿ�ִ��Ŀ���ļ���
gcc -c hello.c��gcc -c hello.c -o hello.o���������ɿ��ض�λĿ���ļ���
*/

 

void main2101()
{
	B b1;
	b1.Set(300);
	b1.printB();
	system("pause");
}
void main2102()
{
	
	A a1(1, 2);
	cout<< a1.getA()<<endl;
	modifyA(&a1, 300);
	cout<< a1.getA()<<endl;

	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}