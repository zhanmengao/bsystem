
#include <iostream>
using namespace std;
#include "dm09_complex.hpp"

void main()
{
	
	//��Ҫ��ģ���� ���о��廯�Ժ�  ���ܶ������  C++������Ҫ�����ڴ�
	Complex<int>	c1(1, 2);
	Complex<int>	c2(3, 4);

	Complex<int> c3 = c1 + c2;
	//c3.printCom();
	cout << c3 << endl;

	//������Ԫ����
	{
		//Complex<int> c4 = MySub<int>(c1, c2);
		//cout << c4 << endl;
	}

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}