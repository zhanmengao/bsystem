
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

	//在圆形1  不在圆内 0
	int tag = c1.judge(p1);
	if (tag == 1)
	{
		cout<<"点在圆形内"<<endl;
	}
	else
	{
		cout<<"点在圆形外"<<endl;
	}

	system("pause");
	return ;
}