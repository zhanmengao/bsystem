#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main101()
{
	int sum=0, value = 0;
	cout << "请输入一些数，输入Ctrl+Z结束输入"<<endl;
	while (cin >> value)
	{
		sum += value;
	}
	cout << "和为"<<sum<<endl;
	return 0;
}