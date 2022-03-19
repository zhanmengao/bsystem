#include<iostream>
#include <string>
using namespace std;
//�������£���������һ����p������жϵ�p�Ƿ��ھ�����?
class Point{
public:
	int x;
	int y;
	Point(int x, int y) :x(x), y(y)
	{

	}
};
//����|p1p2|X|p1p|
int GetCross(const Point& p1,const Point& p2,const Point& p)
{
	return (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
}
class Cube{
private:
	Point a;
	Point b;
	Point c;
	Point d;
public:
	Cube(Point a, Point b, Point c, Point d) :a(a), b(b), c(c), d(d)
	{
		
	}
	bool isContain(const Point &p) const
	{
		//����x,y�����������ھ�����
		return GetCross(a, b, p) * GetCross(c, d, p) >= 0 && GetCross(b, c, p) * GetCross(d,a, p) >= 0;
	}
};
void mainguangyu()
{
	Point a(0, 0);
	Point b(0,1);
	Point c(1, 0);
	Point d(1, 1);
	Cube cu(a, b, c, d);
	bool bo = cu.isContain(a);
	cout << bo;
}
