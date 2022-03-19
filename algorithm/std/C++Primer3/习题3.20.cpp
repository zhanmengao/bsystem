#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main320()
{
	vector<int> v1;
	int temp;
	while (cin >> temp)
	{
		v1.push_back(temp);
		//如果当前是偶数
		if (v1.size() % 2 == 0 && v1.size()!=0)
		{
			cout <<"和为"<<(*(v1.end() - 1)) + (*(v1.end()-2))<<endl;
		}
	}
	//输出第一个和最后一个的和 然后输出第二个和倒数第二个的和
	for (int i = 0; i < v1.size() / 2; i++)
	{
		cout <<i<<"和第"<<v1.size()-i-1<<"的和为"<<v1[i] + v1[v1.size()-1-i]<<endl;
	}
	return 0;
}