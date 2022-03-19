#include "iostream"
using namespace std;

//1文件中iostream 没有引入标准的 std ; 需要我们程序员手工的写
//2 如果不写 using namespace std; 需要显示的引入std

void main301()
{
	std::cout<<"namespace test"<<std::endl;

	system("pause");
}

//3 定义命名空间

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

//4 使用命名空间
//
void main302()
{
	using namespace namespaceA;
	using namespace namespaceB;
	cout<<namespaceA::a<<endl;
	cout<<namespaceB::a<<endl;

	//显示的, 写全
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