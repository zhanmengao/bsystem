#include<iostream>
#include <string>
using namespace std;

void printSanjiao(int n)
{
	//���ѭ������ӡһ��
	for (int i = n; i > 0; i--)
	{
		//��ӡ�ո�
		//for (int j = n; j > ;j--)
		//��ӡ#
		for (int j = 0; j < 2 * i - 1; j++)
		{
			cout << "#";
		}
		cout<<endl;
	}
}
void main22()
{
	printSanjiao(5);
}