#include<iostream>
#include <string>
#include <iomanip>
using namespace std;

void main1()
{
	int i;
	cin >> i;
	
	cout << "������뷽ʽ������������Ϊ12����������Ҫ��ʾ����"<<endl;
	cin >> i;
	cout << setiosflags(ios::left)<<setw(12)<<i<<endl;
	cout << "�԰˽��ƣ�ʮ���ƣ�ʮ�������������"<<endl;
	cin >> i;
	cout << "ʮ����"<<dec << i<<endl;;
	cout<<"16����"<<hex<<i<<endl;
	cout<<"8����"<<oct<<i<<endl;
	cout << "ʵ�ָ�������ָ����ʽ�Ͷ����ʽ������/��� ��ָ������"<<endl;
	cout << ios::scientific << setprecision(2) << i << endl;
	cout << ios::fixed << setprecision(2) << i << endl;


}