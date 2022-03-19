#include <vector>
#include <iostream> 
using namespace std;

//��ģ��
template<typename T, class N> class TestClass
{
public:
	static bool comp(T num1, N num2)
	{
		cout << "standard class template" << endl;
		return (num1 < num2) ? true : false;
	}
};

//�Բ���ģ����������ػ�
template<class N> class TestClass < int, N >
{
public:
	static bool comp(int num1, N num2)
	{
		cout << "partitial specialization" << endl;
		return (num1 < num2) ? true : false;
	}
};

//��ģ������ػ�Ϊָ��
template<typename T, class N> class TestClass < T*, N* >
{
public:
	static bool comp(T* num1, N* num2)
	{
		cout << "new partitial specialization" << endl;
		return (*num1 < *num2) ? true : false;
	}
};

//��ģ������ػ�Ϊ��һ��ģ����
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
	//���÷��ػ��汾
	cout << TestClass<char, char>::comp('0', '1') << endl;

	//���ò���ģ������ػ��汾
	cout << TestClass<int, char>::comp(30, '1') << endl;

	//����ģ������ػ�Ϊָ��汾
	int a = 30;
	char c = '1';
	cout << TestClass<int*, char*>::comp(&a, &c) << endl;

	//����ģ������ػ�Ϊ��һ��ģ����汾
	vector<int> vecLeft{ 0 };
	vector<int> vecRight{ 1, 2, 3 };
	cout << TestClass<vector<int>, vector<int>>::comp(vecLeft, vecRight) << endl;
}
