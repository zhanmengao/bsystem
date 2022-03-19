
#include <iostream>
using namespace std;

class MyPoint
{
public:
	void setPoint(int _x1, int _y1)
	{
		x1 = _x1;
		y1 = _y1;
	}
	int getX1()
	{
		return x1;
	}
	int getY1()
	{
		return y1;
	}

private:
	int x1;
	int y1;
};

class  AdvCircle
{
public:
	void setCircle(int _r, int _x0, int _y0)
	{
		r = _r; x0 = _x0; y0 = _y0;
	}
public:
		int judge(MyPoint &myp)
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
private:
	int r;
	int x0;
	int y0;
};

//MFC cocos2d-x 
void main301()
{
	AdvCircle c1;
	MyPoint p1;

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
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

void main302()
{	
	//定义点
	int x0 = 1;
	int y0 = 1;

	//定义圆心和圆的半径
	int x1 = 2;
	int y1 = 2;

	int r = 3;

	int a = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) - r*r;
	if (a > 0)
	{
		cout<<"点在圆外";
	}
	else
	{
		cout<<"点在圆内";
	}
	system("pause");
}