
#include <iostream>
using namespace std;

#include "AdvCircle.h"
#include "MyPoint.h"

void main1()
{
	Circle c1;
	Point p1;

	c1.setCircle(2, 3, 3);
	p1.setPoint(7, 7);

	//��Բ��1  ����Բ�� 0
	int tag = c1.judge(p1);
	if (tag == 1)
	{
		cout<<"����Բ����"<<endl;
	}
	else
	{
		cout<<"����Բ����"<<endl;
	}

	system("pause");
	return ;
}