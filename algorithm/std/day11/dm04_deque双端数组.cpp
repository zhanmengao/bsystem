
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
	d1.push_back(1);            //β��
	d1.push_back(3);
	d1.push_back(5);

	d1.push_front(-11);        //ͷ��
	d1.push_front(-33);
	d1.push_front(-55);
	cout << endl;

	cout << "ͷ��Ԫ��:" << d1.front() <<endl;    //-55
	cout << "β��Ԫ��:" << d1.back() << endl;    //5
	printD(d1);

	d1.pop_front();
	d1.pop_back();
	printD(d1);
	cout <<endl;

	//���� -33 �������±��ֵ
	deque<int>::iterator it =  find(d1.begin(), d1.end(), -33 );
	if (it != d1.end())
	{
		cout << "-33�����±���" << distance(d1.begin(), it) <<endl;
	}
	else
	{
		cout << "û���ҵ�ֵΪ-33��Ԫ��" << endl;
	}
}