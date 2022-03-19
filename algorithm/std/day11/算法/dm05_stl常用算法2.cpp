
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"
#include "iterator"  //�������������ͷ�ļ�
#include<numeric>

void printV(vector<int> &v);


void printList(list<int> &v);

void showElem(int &n);

class CMyShow
{
public:
	CMyShow();
	void operator()(int &n);
	void printNum();
protected:
private:
	int num;
};


void main44_adjacent_find()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(5);

	//�������ڵ��ظ�Ԫ��
	vector<int>::iterator it =  adjacent_find(v1.begin(), v1.end() );
	if (it == v1.end())
	{
		cout << "û���ҵ� �ظ���Ԫ��" << endl;
	}
	else
	{
		cout << *it << endl;
	}
	int index = distance(v1.begin(), it);
	cout << index << endl;
	
}

// 0 1  2  3 ......n-1
//���ַ� 1K = 1024  10��  �ٶȿ�    
//�����������в���value,�ҵ�����true.���صİ汾ʵ��ָ���ıȽϺ����������ָ�����ж����
void main45_binary_search()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);

	bool b = binary_search(v1.begin(), v1.end(), 7);
	if (b == true)
	{
		cout << "�ҵ���" << endl;
	}
	else
	{
		cout << "û����" << endl;
	}

}

void main46_count()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(7);
	v1.push_back(9);
	v1.push_back(7);

	int num = count(v1.begin(), v1.end(), 7);
	
	cout << num << endl;
	

}

bool GreatThree(int iNum)
{
	if (iNum > 3)
	{
		return true;
	}
	return false;
}
void main46_countif()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(7);
	v1.push_back(9);
	v1.push_back(7);

	int num = count_if(v1.begin(), v1.end(), GreatThree);
	cout << "num:" << num << endl;
}


void main47_find_findif()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(7);
	v1.push_back(9);
	v1.push_back(7);

	vector<int>::iterator it =  find(v1.begin(), v1.end(), 5);
	cout << "*it:" << *it << endl; 

	//��һ������3��λ��
	vector<int>::iterator it2 =  find_if(v1.begin(), v1.end(), GreatThree);
	cout << "*it2:" << *it2 << endl; 
}

//merge �ϲ�
void main_merge()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);

	vector<int> v3;
	v3.resize(v1.size() + v2.size() );

	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin() );
	
	printV(v3);
}

class Student
{
public:
	Student(string name, int id)
	{
		m_name = name;
		m_id = id;
	}
	void printT()
	{
		cout << "name: " << m_name << " id " << m_id << endl;
	}
public:
	string	m_name;
	int		m_id;
};

bool CompareS(Student &s1, Student &s2)
{
	return (s1.m_id < s2.m_id);
}


void main_sort()
{
	Student s1("�ϴ�", 1);
	Student s2("�϶�", 2);
	Student s3("����", 3);
	Student s4("����", 4);
	vector<Student> v1;
	v1.push_back(s4);
	v1.push_back(s1);
	v1.push_back(s3);
	v1.push_back(s2);

	for (vector<Student>::iterator it=v1.begin(); it!=v1.end(); it++)
	{
		it->printT() ;
	}

	//sort �����Զ��庯������ �����Զ����������͵����� 
	//�滻 �㷨��ͳһ�� (ʵ�ֵ��㷨���������͵ķ���) ===>�����ֶκ�������
	sort(v1.begin(), v1.end(), CompareS );

	for (vector<Student>::iterator it=v1.begin(); it!=v1.end(); it++)
	{
		it->printT() ;
	}

}

//_random_shuffle() ��ָ����Χ�ڵ�Ԫ����������������ذ汾����һ���������������
void main_random_shuffle()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);

	random_shuffle(v1.begin(), v1.end());
	printV(v1);

	string str = "abcdefg";
	random_shuffle(str.begin(), str.end());
	cout << "str: " << str << endl;
}

//_reverse ��ָ����Χ��Ԫ�����·�������
void main_reverse()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	reverse(v1.begin(), v1.end());
	printV(v1);
}

//_copy ������1�����ݿ���������2
void main52_copy()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);

	vector<int> v2;
	v2.resize(v1.size() );

	copy(v1.begin(), v1.end(), v2.begin());
	printV(v2);
}


bool great_equal_5(int &n)
{
	if (n>=5)
	{
		return true;
	}
	return false;
}

//_replace_replaceif �滻
void main_replace_replaceif()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(3);
	replace(v1.begin(), v1.end(), 3, 8);    //��8�滻��8      
	printV(v1);
	cout << endl;
	// >=5
	replace_if(v1.begin(), v1.end(), great_equal_5, 1);       //�Ѵ���5���滻Ϊ1

	printV(v1);

}

//swap:����v1 v2��ֵ
void main54_swap()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);
	

	swap(v1, v2);    
	printV(v1);

}

//accumulate ���
void main55_accumulate()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	int tmp = accumulate(v1.begin(), v1.end(), 100);       //��v1������ͣ���ֵΪ100
	cout << tmp << endl;
}

//fill ���
void main56_fill()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	fill(v1.begin(), v1.end(), 8);     //��v1���������ȫ��Ϊ8
	printV(v1);
	
}

//_union : ����һ���������У������������������еĲ��ظ�Ԫ�ء����ذ汾ʹ���Զ���ıȽϲ���
void main_union()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(2);
	v1.push_back(4);
	v1.push_back(6);

	vector<int> v2;
	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);

	vector<int> v3;
	v3.resize(v1.size() + v2.size());


	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	printV(v3);
}