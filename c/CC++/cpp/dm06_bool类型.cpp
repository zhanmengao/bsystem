
#include <iostream>
using namespace std;


void main601()
{
	bool b1 = true;  //告诉c++编译器给我分配 1个字节的内存


	bool b2, b3, b4, b5;
	//

	cout<<"sizeof(bool)"<<sizeof(bool)<<endl;
	
	//bool变量 1 或者 0
	b1 = 10;
	cout<<"bl:" <<b1<<endl;

	//bool变量 1 或者 0
	b1 = -10;
	cout<<"bl:" <<b1<<endl;

	//bool变量 1 或者 0
	b1 = 0;
	cout<<"bl:" <<b1<<endl;

	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}