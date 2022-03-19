
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"
#include "iterator"  //�������������ͷ�ļ�

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

//foreach:�Լ�����ÿ����Աִ����ͬ����
void main41_foreach()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;

	//�������� �ص�������ڵ�ַ
	for_each(v1.begin(), v1.end(), showElem);             //����ָ�뷽ʽ
	cout << endl;
	for_each(v1.begin(), v1.end(), CMyShow());            //��������ʽ
	cout << endl;

	 CMyShow mya;
	 CMyShow my1 = for_each(v1.begin(), v1.end(),mya); //��my1��ʼ��
	 mya.printNum();  //ma1��my1 ��������ͬ�Ķ���
	 my1.printNum();

	 my1 = for_each(v1.begin(), v1.end(),mya);  //��my1��ֵ
	 my1.printNum();
}


int  increase(int i)
{
	return i+100;
}

//transform���Լ��Ͻ��в���������ŵ���һ������
void main42_transform()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;

	//transform ʹ�ûص�����
	transform(v1.begin(), v1.end(), v1.begin(),  increase ); 
	printV(v1);
	cout << endl;

	//transform ʹ�� Ԥ����ĺ�������
	transform(v1.begin(), v1.end(), v1.begin(),  negate<int>() ); 
	printV(v1);
	cout << endl;

	//transform ʹ�� ���������� �ͺ�������
	list<int> mylist;
	mylist.resize( v1.size() );

	transform(v1.begin(), v1.end(), mylist.begin(),  bind2nd( multiplies<int>(), 10 ) ); 
	printList(mylist);
	cout << endl;

	//transform Ҳ���԰������� ֱ���������Ļ
	transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " " ), negate<int>() );
	cout << endl;
}



int showElem2(int n)
{
	cout << n << " ";
	return n;
}

//һ������£�for_each��ʹ�õĺ������󣬲��������ã�û�з���ֵ
//transform��ʹ�õĺ������󣬲���һ�㲻ʹ�����ã����ǻ��з���ֵ
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
