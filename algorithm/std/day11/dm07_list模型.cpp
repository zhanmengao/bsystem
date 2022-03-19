
#include <iostream>
using namespace std;
#include "list"
//双向链表


void printD(list<int> &d)
{
	for (list<int>::iterator it = d.begin(); it != d.end(); it++)
	{
		cout << *it << " ";
	}
}

//list的基本操作
void main701()
{
	list<int> l;
	cout <<  "list的大小:" << l.size() << endl;
	for (int i=0; i<10; i++)
	{
		l.push_back(i); //尾部插入元素 尾插法
	}
	cout <<  "list的大小:" << l.size() << endl;

	list<int>::iterator it = l.begin();

	//遍历双链表
	while (it != l.end())
	{
		cout << *it << " ";
		it ++;
	}
	cout << endl;

	//list不能随机访问
	//0	 1	2	3	4	5
//              ▲
	it = l.begin();
	it ++;
	it ++ ;
	it ++ ;
	//it = it + 5;  error//不支持随机的访问容器

	//根据it插入
	l.insert(it, 100); //请问100插入在什么位置：在it之前,it被挤到后面
	printD(l);
	//结论1 链表的结点index 序号是从0号位置开始
	//		在3号位置插入元素, 让原来的3号位置变成4号位置  原来的4号位置变成5号位置


	//头部插入元素 头插法
	for (int i = 0; i < 10; i++)
	{
		l.push_front(i); 
	}
	cout << "list的大小:" << l.size() << endl;

	printD(l);
	cout << endl;
}

//list 删除
void main702()
{
	list<int> l;
	list<int>::iterator it1 = l.begin();
	list<int>::iterator it2 = l.begin();
	it2 ++ ;
	it2 ++ ;
	it2 ++ ;

	l.erase(it1, it2);  //删除it1->it2范围内的元素
	printD(l);
	cout << endl;

	l.insert(l.begin(), 100);   //在头部插入100
	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);

	l.erase(l.begin());     //删除头部元素
	l.remove(100); //删除第一个找到的100
	printD(l);
	cout << endl;
}