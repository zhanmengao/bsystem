
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"

//�������� ��������()
template <typename T>
class ShowElemt
{
public:
	ShowElemt()
	{
		n = 0;
	}
	void operator()(T &t)
	{
		n ++;
		//printN();
		cout << t << " ";
	}
	void printN()
	{
		cout << "n:" << n << endl;
	}
protected:
private:
	int n;  //����״̬������
};


//����ģ��  ==����
template <typename T>
void FuncShowElemt(T &t)
{
	cout << t << endl;
}

//��ͨ����
void FuncShowElemt2(int &t)
{
	cout << t << " ";
}

//�������� ���� ;  �����������ͨ��������ͬ 
void main01()
{
	int a = 10;
	ShowElemt<int> showElemt;
	showElemt(a); //���������operator()��ִ�� ����һ������ //�º���

	FuncShowElemt<int>(a);
	FuncShowElemt2(a);
}

//�������������������,��ͻ�ƺ����ĸ���,�ܱ��ֵ���״̬��Ϣ
//��������ĺô�
// for_each�㷨��, ������������������
// for_each�㷨��, �������󵱷���ֵ
void main02()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	for_each(v1.begin(), v1.end(), ShowElemt<int>()); //ʹ�������������� �����º���
	cout << endl;
	for_each(v1.begin(), v1.end(), FuncShowElemt2); //ͨ���ص�����  ˭ʹ��for_each ˭ȥ��д�ص���������ڵ�ַ


	ShowElemt<int> show1;
	//�������� ���������� 
	//1 for_each�㷨�� ��������Ĵ��� ��Ԫ��ֵ���� ,�������ô���
	for_each(v1.begin(), v1.end(), show1);
	show1.printN();

	cout << "ͨ��for_each�㷨�ķ���ֵ�����õĴ���" << endl;
	show1 = for_each(v1.begin(), v1.end(), show1);
	show1.printN();

	//���� Ҫ��: ����� stl�㷨���ص�ֵ�ǵ����� ���� ν�ʣ��������� ��stl�㷨���ŵ���Ҫ��
}

template<typename T>
class IsDiv
{
public:
	IsDiv(const T &divisor)
	{
		this->divisor = divisor;
	}
	bool operator()(T  &t)
	{
		return (t%divisor == 0);
	}

protected:
private:
	T divisor;
};

//һԪν��     find_if����ֵ��һ�������� 
void main303()
{
	vector<int> v2;
	for (int i=10; i<33; i++)
	{
		v2.push_back(i);
	}
	int a = 4;
	IsDiv<int> myDiv(a);

	//find_if(v2.begin(), v2.end(), myDiv );

	vector<int>::iterator it;
	it = find_if(v2.begin(), v2.end(), IsDiv<int>(a) );
	if (it == v2.end())
	{
		cout << "������û�б�4������Ԫ��" << endl;
	}
	else
	{
		cout <<"��һ���Ǳ�4������Ԫ����:" << *it << endl;
	}

}


//��Ԫ��������
template <typename T>
class SumAdd
{
public:
	T operator()(T t1, T t2)
	{
		return t1 + t2;
	}
};


//��Ԫ�������� �Ͷ�Ԫν��          transform ���������� �������Ŀ�ʼλ�� ���س��� 
void main04()
{
	//v1 v2 ==> v3
	vector<int> v1, v2;
	vector<int> v3;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);

	v3.resize(10);

	transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), SumAdd<int>() );     //��Ԫtransform:��v1��v2��Ӧλ�õ���SumAdd������ŵ�v3

	for (vector<int>::iterator it=v3.begin(); it!=v3.end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl;
}


bool MyCompare(const int &a, const int &b)
{
	return a < b; //��С����
}

//main05(); //��Ԫ�������� �Ͷ�Ԫν��
void main05()
{
	vector<int> v1(10);

	for (int i=0; i<10; i++)
	{
		int tmp = rand() %100;
		v1[i] = tmp;
	}

	cout << endl;
	for_each(v1.begin(), v1.end(), FuncShowElemt2);
	cout << endl;

	sort(v1.begin(), v1.end(), MyCompare);
	for_each(v1.begin(), v1.end(), FuncShowElemt2);
	cout << endl;
}

struct CompareNoCase
{
	bool operator()(const string &str1, const string &str2)
	{
		string str1_ ;
		str1_.resize(str1.size() );
		transform(str1.begin(), str1.end(), str1_.begin(), tolower ); //һԪtransform,��str1�������ַ� ת��ΪСд���ŵ�temp1

		string str2_ ;
		str2_.resize(str2.size() );
		transform(str2.begin(), str2.end(), str2_.begin(), tolower ); //һԪtransform,��str2�������ַ� ת��Ϊ��д���ŵ�temp2

		return (str1_ < str2_); // ��С�����������
	}
};


//��Ԫν����set�����е�Ӧ��
void  main06()
{
	set<string> set1;
	set1.insert("bbb");
	set1.insert("aaa");
	set1.insert("ccc");
	set<string>::iterator it = set1.find("aAa"); //find���� Ĭ�� ���ִ�Сд
	if (it == set1.end())
	{
		cout << " û�� ���ҵ� aaa " << endl;
	}
	else
	{
		cout << " ���ҵ� aaa " << endl;
	}

	set<string, CompareNoCase> set2;           //��ν�ʵ�set
	set2.insert("bbb");
	set2.insert("aaa");
	set2.insert("ccc");

	set<string, CompareNoCase>::iterator it2 = set2.find("aAa");
	if (it2 == set2.end())
	{
		cout << " û�� ���ҵ� aaa " << endl;
	}
	else
	{
		cout << " �����ִ�С�ĵĲ���  ���ҵ� aaa " << endl;
	}

}
