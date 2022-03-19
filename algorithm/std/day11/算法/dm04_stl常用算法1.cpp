
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"
#include "iterator"  //输出流迭代器的头文件

void printV(vector<int> &v)
{
	for (vector<int>::iterator it=v.begin(); it!=v.end(); it++)
	{
		cout << *it << " ";
	}
}

void printList(list<int> &v)
{
	for (list<int>::iterator it=v.begin(); it!=v.end(); it++)
	{
		cout << *it << " ";
	}
}

void showElem(int &n)
{
	cout << n << " ";
}

class CMyShow
{
public:
	CMyShow()
	{
		num = 0;
	}
	void operator()(int &n)
	{
		num ++;
		cout << n << " ";
	}
	void printNum()
	{
		cout << "num:" << num << endl;
	}
protected:
private:
	int num;
};

//foreach:对集合中每个成员执行相同操作
void main41_foreach()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;

	//函数对象 回调函数入口地址
	for_each(v1.begin(), v1.end(), showElem);             //函数指针方式
	cout << endl;
	for_each(v1.begin(), v1.end(), CMyShow());            //函数对象方式
	cout << endl;

	 CMyShow mya;
	 CMyShow my1 = for_each(v1.begin(), v1.end(),mya); //给my1初始化
	 mya.printNum();  //ma1和my1 是两个不同的对象
	 my1.printNum();

	 my1 = for_each(v1.begin(), v1.end(),mya);  //给my1赋值
	 my1.printNum();
}


int  increase(int i)
{
	return i+100;
}

//transform：对集合进行操作，结果放到另一个集合
void main42_transform()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;

	//transform 使用回调函数
	transform(v1.begin(), v1.end(), v1.begin(),  increase ); 
	printV(v1);
	cout << endl;

	//transform 使用 预定义的函数对象
	transform(v1.begin(), v1.end(), v1.begin(),  negate<int>() ); 
	printV(v1);
	cout << endl;

	//transform 使用 函数适配器 和函数对象
	list<int> mylist;
	mylist.resize( v1.size() );

	transform(v1.begin(), v1.end(), mylist.begin(),  bind2nd( multiplies<int>(), 10 ) ); 
	printList(mylist);
	cout << endl;

	//transform 也可以把运算结果 直接输出到屏幕
	transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " " ), negate<int>() );
	cout << endl;
}



int showElem2(int n)
{
	cout << n << " ";
	return n;
}

//一般情况下：for_each所使用的函数对象，参数是引用，没有返回值
//transform所使用的函数对象，参数一般不使用引用，而是还有返回值
void main43_transform_pk_foreach()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2 = v1;

	for_each(v1.begin(), v1.end(), showElem);
	auto ret = transform(v2.begin(), v2.end(), v2.begin(), showElem2);
}
