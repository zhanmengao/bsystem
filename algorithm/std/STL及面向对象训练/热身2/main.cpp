/*
2�����һ����ģ�壬���а������ݳ�ԱT a[n]�Լ������н��в�������Ԫ�صĺ���int search(T),
ģ����� TҪ����֧���ַ�����
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
	A<int, 5> intAry;		//��intʵ����������ģ�������
	A<double, 10> douAry;//��doubleʵ����������ģ�������
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