#include "iostream"
using namespace std;

//1�ļ���iostream û�������׼�� std ; ��Ҫ���ǳ���Ա�ֹ���д
//2 �����д using namespace std; ��Ҫ��ʾ������std

void main301()
{
	std::cout<<"namespace test"<<std::endl;

	system("pause");
}

//3 ���������ռ�

namespace  namespaceA
{
	int a = 10;
}

namespace  namespaceB
{
	int a = 20;
	namespace namespaceC
	{
		struct Teacher
		{
			char name[32];
			int age ;
		};
	}
}

//4 ʹ�������ռ�
//
void main302()
{
	using namespace namespaceA;
	using namespace namespaceB;
	cout<<namespaceA::a<<endl;
	cout<<namespaceB::a<<endl;

	//��ʾ��, дȫ
	{
		//namespaceB::namespaceC::Teacher t1;
		//t1.age = 33;
	}
	

	//

	using namespaceB::namespaceC::Teacher  ;
	Teacher t2;
	t2.age = 36;


	system("pause");
}