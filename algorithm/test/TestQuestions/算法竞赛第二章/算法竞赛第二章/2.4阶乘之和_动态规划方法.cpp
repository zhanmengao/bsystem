#include<iostream>
#include <string>
using namespace std;
/*
��̬�滮����˼�� �� ��̬�滮�ǰ�һ�����������һ��С���⡣
һ��������֮�������á���̬�滮���������������Ϊ���ܲ���һ��С���⣬��ʵ��ɶ�����ⶼ�ܲ���С���⡣
ȡ���ڸ������Ƿ����ö�̬�滮���������Щ��С���⡰�᲻�ᱻ���ظ����á�
*/
long long *arr;
int jiecheng(int n)
{
	if (n > 1e6)
	{
		return 0;
	}
	//��ʼ��
	arr = new long long[n+1];
	arr[0] = 1;
	arr[1] = 1;
	for (int i = 2; i < n; i++)
	{
		arr[i] = 0;
	}
	//�����
	long long total = 0;
	//�Ե�����
	for (int i = 2; i <= n; i++)
	{
		arr[i] = i*arr[i - 1];
	}
	for (int i = 0; i <= n; i++)
	{
		cout <<i<<"!="<< arr[i]<<endl;
		total += arr[i];\
	}
	return total;
}

int jiecheng2(int n)
{
	//���ֵ����
	if (arr[n] != 0)
	{
		return arr[n];
	}
	//��������� �������
	else
	{
		arr[n] = n*jiecheng2(n-1);
	}
	return arr[n];
}

void main2444()
{
	int n = 0;
	cin >> n;
	cout<<n<<"�Ľ׳�֮��Ϊ"<<jiecheng(n)<<endl;
}

