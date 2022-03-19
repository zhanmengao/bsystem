#include <vector>
#include <iostream> 
using namespace std;
/*
ģ��ƫ�ػ���Template Partitial Specialization����ģ���ػ���һ�����������
ָ��ʾָ������ģ���������ȫ��ģ�����������ָ��ģ������Ĳ������Էֶ���ȫ�����ԣ�Ҳ��Ϊģ�岿���ػ���
��ģ��ƫ�ػ���Ե���ģ��ȫ�ػ���ָ�����е�ģ����������ػ���ģ��ȫ�ػ���ģ��ƫ�ػ���ͬ���ģ���ػ���

ģ��ƫ�ػ���Ҫ��Ϊ���֣�һ����ָ�Բ���ģ���������ȫ�ػ�����һ���Ƕ�ģ��������Խ����ػ���������ģ������ػ�Ϊָ�롢���û�������һ��ģ���ࡣ
*/
//����ģ��
template<typename T, class N> void compare(T num1, N num2)
{
	cout << "standard function template" << endl;
	if (num1 > num2)
		cout << "num1:" << num1 << " > num2:" << num2 << endl;
	else
		cout << "num1:" << num1 << " <= num2:" << num2 << endl;
}

//�Բ���ģ����������ػ�
template<class N> void compare(int num1, N num2)
{
	cout << "partitial specialization" << endl;
	if (num1 > num2)
		cout << "num1:" << num1 << " > num2:" << num2 << endl;
	else
		cout << "num1:" << num1 << " <= num2:" << num2 << endl;
}

//��ģ������ػ�Ϊָ��
template<typename T, class N> void compare(T* num1, N* num2)
{
	cout << "new partitial specialization" << endl;
	if (*num1 > *num2)
		cout << "num1:" << *num1 << " > num2:" << *num2 << endl;
	else
		cout << "num1:" << *num1 << " <= num2:" << *num2 << endl;
}

//��ģ������ػ�Ϊ��һ��ģ����
template<typename T, class N> void compare(std::vector<T>& vecLeft, std::vector<T>& vecRight)
{
	cout << "to vector partitial specialization" << endl;
	if (vecLeft.size() > vecRight.size())
		cout << "vecLeft.size()" << vecLeft.size() << " > vecRight.size():" << vecRight.size() << endl;
	else
		cout << "vecLeft.size()" << vecLeft.size() << " <= vecRight.size():" << vecRight.size() << endl;
}

void mainFunc()
{
	compare<int, int>(30, 31);//���÷��ػ��汾compare<int,int>(int num1, int num2)

	compare(30, '1');		//����ƫ�ػ��汾compare<char>(int num1, char num2)

	int a = 30;
	char c = '1';
	compare(&a, &c);		//����ƫ�ػ��汾compare<int,char>(int* num1, char* num2)

	vector<int> vecLeft{ 0 };
	vector<int> vecRight{ 1, 2, 3 };
	compare<int, int>(vecLeft, vecRight);	//����ƫ�ػ��汾compare<int,char>(int* num1, char* num2)
}
