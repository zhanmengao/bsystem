
#include <iostream>
using namespace std;
#include "deque"
#include "algorithm"


void printD(deque<int> &d)
{
	for (deque<int>::iterator it=d.begin(); it!=d.end(); it++ )
	{
		cout << *it << " ";
	}
}
void main4011()
{
	deque<int> d1;
	d1.push_back(1);            //尾插
	d1.push_back(3);
	d1.push_back(5);

	d1.push_front(-11);        //头插
	d1.push_front(-33);
	d1.push_front(-55);
	cout << endl;

	cout << "头部元素:" << d1.front() <<endl;    //-55
	cout << "尾部元素:" << d1.back() << endl;    //5
	printD(d1);

	d1.pop_front();
	d1.pop_back();
	printD(d1);
	cout <<endl;

	//查找 -33 在数组下标的值
	deque<int>::iterator it =  find(d1.begin(), d1.end(), -33 );
	if (it != d1.end())
	{
		cout << "-33数组下标是" << distance(d1.begin(), it) <<endl;
	}
	else
	{
		cout << "没有找到值为-33的元素" << endl;
	}
}