#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main101()
{
	int sum=0, value = 0;
	cout << "������һЩ��������Ctrl+Z��������"<<endl;
	while (cin >> value)
	{
		sum += value;
	}
	cout << "��Ϊ"<<sum<<endl;
	return 0;
}