
#include <iostream>
using namespace std;
#include "list"
//˫������


void printD(list<int> &d)
{
	for (list<int>::iterator it = d.begin(); it != d.end(); it++)
	{
		cout << *it << " ";
	}
}

//list�Ļ�������
void main701()
{
	list<int> l;
	cout <<  "list�Ĵ�С:" << l.size() << endl;
	for (int i=0; i<10; i++)
	{
		l.push_back(i); //β������Ԫ�� β�巨
	}
	cout <<  "list�Ĵ�С:" << l.size() << endl;

	list<int>::iterator it = l.begin();

	//����˫����
	while (it != l.end())
	{
		cout << *it << " ";
		it ++;
	}
	cout << endl;

	//list�����������
	//0	 1	2	3	4	5
//              ��
	it = l.begin();
	it ++;
	it ++ ;
	it ++ ;
	//it = it + 5;  error//��֧������ķ�������

	//����it����
	l.insert(it, 100); //����100������ʲôλ�ã���it֮ǰ,it����������
	printD(l);
	//����1 ����Ľ��index ����Ǵ�0��λ�ÿ�ʼ
	//		��3��λ�ò���Ԫ��, ��ԭ����3��λ�ñ��4��λ��  ԭ����4��λ�ñ��5��λ��


	//ͷ������Ԫ�� ͷ�巨
	for (int i = 0; i < 10; i++)
	{
		l.push_front(i); 
	}
	cout << "list�Ĵ�С:" << l.size() << endl;

	printD(l);
	cout << endl;
}

//list ɾ��
void main702()
{
	list<int> l;
	list<int>::iterator it1 = l.begin();
	list<int>::iterator it2 = l.begin();
	it2 ++ ;
	it2 ++ ;
	it2 ++ ;

	l.erase(it1, it2);  //ɾ��it1->it2��Χ�ڵ�Ԫ��
	printD(l);
	cout << endl;

	l.insert(l.begin(), 100);   //��ͷ������100
	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);

	l.erase(l.begin());     //ɾ��ͷ��Ԫ��
	l.remove(100); //ɾ����һ���ҵ���100
	printD(l);
	cout << endl;
}