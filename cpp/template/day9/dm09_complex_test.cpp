
#include <iostream>
using namespace std;
#include "dm09_complex.hpp"

void main()
{
	
	//需要把模板类 进行具体化以后  才能定义对象  C++编译器要分配内存
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//滥用友元函数
	{
		//Complex<int> c4 = MySub<int>(c1, c2);
		//cout << c4 << endl;
	}

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}