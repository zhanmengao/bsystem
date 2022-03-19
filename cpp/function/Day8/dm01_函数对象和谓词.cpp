
#include <iostream>
using namespace std;

#include "string"
#include <vector>
#include <list>
#include "set"
#include <algorithm>
#include "functional"

//函数对象 类重载了()
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
	int n;  //调用状态：计数
};


//函数模板  ==函数
template <typename T>
void FuncShowElemt(T &t)
{
	cout << t << endl;
}

//普通函数
void FuncShowElemt2(int &t)
{
	cout << t << " ";
}

//函数对象 定义 ;  函数对象和普通函数的异同 
void main01()
{
	int a = 10;
	ShowElemt<int> showElemt;
	showElemt(a); //函数对象的operator()的执行 很像一个函数 //仿函数

	FuncShowElemt<int>(a);
	FuncShowElemt2(a);
}

//函数对象是属于类对象,能突破函数的概念,能保持调用状态信息
//函数对象的好处
// for_each算法中, 函数对象做函数参数
// for_each算法中, 函数对象当返回值
void main02()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	for_each(v1.begin(), v1.end(), ShowElemt<int>()); //使用匿名函数对象 匿名仿函数
	cout << endl;
	for_each(v1.begin(), v1.end(), FuncShowElemt2); //通过回调函数  谁使用for_each 谁去填写回调函数的入口地址


	ShowElemt<int> show1;
	//函数对象 做函数参数 
	//1 for_each算法的 函数对象的传递 是元素值传递 ,不是引用传递
	for_each(v1.begin(), v1.end(), show1);
	show1.printN();

	cout << "通过for_each算法的返回值看调用的次数" << endl;
	show1 = for_each(v1.begin(), v1.end(), show1);
	show1.printN();

	//结论 要点: 分清楚 stl算法返回的值是迭代器 还是 谓词（函数对象） 是stl算法入门的重要点
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

//一元谓词     find_if返回值是一个迭代器 
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
		cout << "容器中没有被4整除的元素" << endl;
	}
	else
	{
		cout <<"第一个是被4整除的元素是:" << *it << endl;
	}

}


//二元函数对象
template <typename T>
class SumAdd
{
public:
	T operator()(T t1, T t2)
	{
		return t1 + t2;
	}
};


//二元函数对象 和二元谓词          transform 把运算结果的 迭代器的开始位置 返回出来 
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

	transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), SumAdd<int>() );     //二元transform:将v1和v2对应位置调用SumAdd，结果放到v3

	for (vector<int>::iterator it=v3.begin(); it!=v3.end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl;
}


bool MyCompare(const int &a, const int &b)
{
	return a < b; //从小到大
}

//main05(); //二元函数对象 和二元谓词
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
		transform(str1.begin(), str1.end(), str1_.begin(), tolower ); //一元transform,将str1的所有字符 转换为小写，放到temp1

		string str2_ ;
		str2_.resize(str2.size() );
		transform(str2.begin(), str2.end(), str2_.begin(), tolower ); //一元transform,将str2的所有字符 转换为大写，放到temp2

		return (str1_ < str2_); // 从小到大进行排序
	}
};


//二元谓词在set集合中的应用
void  main06()
{
	set<string> set1;
	set1.insert("bbb");
	set1.insert("aaa");
	set1.insert("ccc");
	set<string>::iterator it = set1.find("aAa"); //find函数 默认 区分大小写
	if (it == set1.end())
	{
		cout << " 没有 查找到 aaa " << endl;
	}
	else
	{
		cout << " 查找到 aaa " << endl;
	}

	set<string, CompareNoCase> set2;           //带谓词的set
	set2.insert("bbb");
	set2.insert("aaa");
	set2.insert("ccc");

	set<string, CompareNoCase>::iterator it2 = set2.find("aAa");
	if (it2 == set2.end())
	{
		cout << " 没有 查找到 aaa " << endl;
	}
	else
	{
		cout << " 不区分大小的的查找  查找到 aaa " << endl;
	}

}
