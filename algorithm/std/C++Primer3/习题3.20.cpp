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
		//�����ǰ��ż��
		if (v1.size() % 2 == 0 && v1.size()!=0)
		{
			cout <<"��Ϊ"<<(*(v1.end() - 1)) + (*(v1.end()-2))<<endl;
		}
	}
	//�����һ�������һ���ĺ� Ȼ������ڶ����͵����ڶ����ĺ�
	for (int i = 0; i < v1.size() / 2; i++)
	{
		cout <<i<<"�͵�"<<v1.size()-i-1<<"�ĺ�Ϊ"<<v1[i] + v1[v1.size()-1-i]<<endl;
	}
	return 0;
}