#include "iostream"
using namespace std;

#include "AdvCircle.h"
#include "MyPoint.h"

void Circle::setCircle(int _r, int _x0, int _y0)
{
	r = _r; x0 = _x0; y0 = _y0;
}

int Circle::judge(Point &myp)
{
	int dd = (myp.getX1() - x0 )*(myp.getX1() - x0 ) + (myp.getY1()-y0)* (myp.getY1()-y0);
	if (dd <= r*r)
	{
		return 1; //圆形内部
	}
	else
	{
		return 0; //0点在圆外
	}
}
