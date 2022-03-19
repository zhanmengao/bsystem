
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"
#include "iterator"  //输出流迭代器的头文件
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

	//查找相邻的重复元素
	vector<int>::iterator it =  adjacent_find(v1.begin(), v1.end() );
	if (it == v1.end())
	{
		cout << "没有找到 重复的元素" << endl;
	}
	else
	{
		cout << *it << endl;
	}
	int index = distance(v1.begin(), it);
	cout << index << endl;
	
}

// 0 1  2  3 ......n-1
//二分法 1K = 1024  10次  速度快    
//在有序序列中查找value,找到返回true.重载的版本实用指定的比较函数对象或函数指针来判断相等
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
		cout << "找到了" << endl;
	}
	else
	{
		cout << "没到了" << endl;
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

	//第一个大于3的位置
	vector<int>::iterator it2 =  find_if(v1.begin(), v1.end(), GreatThree);
	cout << "*it2:" << *it2 << endl; 
}

//merge 合并
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
	Student s1("老大", 1);
	Student s2("老二", 2);
	Student s3("老三", 3);
	Student s4("老四", 4);
	vector<Student> v1;
	v1.push_back(s4);
	v1.push_back(s1);
	v1.push_back(s3);
	v1.push_back(s2);

	for (vector<Student>::iterator it=v1.begin(); it!=v1.end(); it++)
	{
		it->printT() ;
	}

	//sort 根据自定义函数对象 进行自定义数据类型的排序 
	//替换 算法的统一性 (实现的算法和数据类型的分离) ===>技术手段函数对象
	sort(v1.begin(), v1.end(), CompareS );

	for (vector<Student>::iterator it=v1.begin(); it!=v1.end(); it++)
	{
		it->printT() ;
	}

}

//_random_shuffle() 对指定范围内的元素随机调整次序。重载版本输入一个随机数产生操作
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

//_reverse 将指定范围内元素重新反序排序
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

//_copy 将集合1的数据拷贝到集合2
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

//_replace_replaceif 替换
void main_replace_replaceif()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(3);
	replace(v1.begin(), v1.end(), 3, 8);    //把8替换成8      
	printV(v1);
	cout << endl;
	// >=5
	replace_if(v1.begin(), v1.end(), great_equal_5, 1);       //把大于5的替换为1

	printV(v1);

}

//swap:交换v1 v2的值
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

//accumulate 求和
void main55_accumulate()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	int tmp = accumulate(v1.begin(), v1.end(), 100);       //对v1进行求和，初值为100
	cout << tmp << endl;
}

//fill 填充
void main56_fill()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	fill(v1.begin(), v1.end(), 8);     //将v1里面的数据全变为8
	printV(v1);
	
}

//_union : 构造一个有序序列，包含两个序列中所有的不重复元素。重载版本使用自定义的比较操作
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