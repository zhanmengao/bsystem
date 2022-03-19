#include <vector>
#include <iostream> 
using namespace std;

//类模板
template<typename T, class N> class TestClass
{
public:
	static bool comp(T num1, N num2)
	{
		cout << "standard class template" << endl;
		return (num1 < num2) ? true : false;
	}
};

//对部分模板参数进行特化
template<class N> class TestClass < int, N >
{
public:
	static bool comp(int num1, N num2)
	{
		cout << "partitial specialization" << endl;
		return (num1 < num2) ? true : false;
	}
};

//将模板参数特化为指针
template<typename T, class N> class TestClass < T*, N* >
{
public:
	static bool comp(T* num1, N* num2)
	{
		cout << "new partitial specialization" << endl;
		return (*num1 < *num2) ? true : false;
	}
};

//将模板参数特化为另一个模板类
template<typename T, class N> class TestClass < vector<T>, vector<N> >
{
public:
	static bool comp(const vector<T>& vecLeft, const vector<N>& vecRight)
	{
		cout << "to vector partitial specialization" << endl;
		return (vecLeft.size() < vecRight.size()) ? true : false;
	}
};

void main()
{
	//调用非特化版本
	cout << TestClass<char, char>::comp('0', '1') << endl;

	//调用部分模板参数特化版本
	cout << TestClass<int, char>::comp(30, '1') << endl;

	//调用模板参数特化为指针版本
	int a = 30;
	char c = '1';
	cout << TestClass<int*, char*>::comp(&a, &c) << endl;

	//调用模板参数特化为另一个模板类版本
	vector<int> vecLeft{ 0 };
	vector<int> vecRight{ 1, 2, 3 };
	cout << TestClass<vector<int>, vector<int>>::comp(vecLeft, vecRight) << endl;
}
