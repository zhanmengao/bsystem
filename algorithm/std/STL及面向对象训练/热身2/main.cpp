/*
2、设计一个类模板，其中包括数据成员T a[n]以及在其中进行查找数据元素的函数int search(T),
模板参数 T要可以支持字符串。
*/
#include <iostream>
using namespace std;

template<typename T, int n>
class A{
	int size;
	T* element;
public:
	A();
	~A();
	int Search(T);
	void SetElement(int index, const T& value);
};

template<typename T, int n>
A<T, n>::A(){
	size = n > 1 ? n : 1;
	element = new T[size];
}

template<typename T, int n>
A<T, n>::~A(){
	delete[] element;
}

template<typename T, int n>
int A<T, n>::Search(T t){
	int i;
	for (i = 0; i < size; i++)
	if (element[i] == t)
		return i;
	return -1;
}

template<typename T, int n>
void A<T, n>::SetElement(int index, const T& value){
	element[index] = value;
}

int main(int argc, char **argv)
{
	A<int, 5> intAry;		//用int实例化，建立模板类对象
	A<double, 10> douAry;//用double实例化，建立模板类对象
	int i;
	for (i = 0; i < 5; i++)
	{
		intAry.SetElement(i, i + 3);
	}
	
	for (i = 0; i < 10; i++)
	{
		douAry.SetElement(i, (i + i)*0.35);
	}
	
	i = intAry.Search(7);
	if (i >= 0)cout << i << endl;
	{
		i = douAry.Search(0.7);
	}
	
	if (i >= 0)
	{
		cout << i << endl;
	}
	system("pause");
	return 0;
}