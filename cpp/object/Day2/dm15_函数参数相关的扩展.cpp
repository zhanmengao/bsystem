
#include <iostream>
using namespace std;

void myPrint15(int x = 3)
{
	cout<<"x"<<x<<endl;
}

//1 �� ����д����,ʹ������д��,����дĬ��
//2 ��Ĭ�ϲ������� �����Ĭ�ϲ������֣���ô�ұߵĶ�������Ĭ�ϲ���
void myPrint2( int m, int n, int x = 3, int y = 4)
//void myPrint2( int m, int n, int x = 3, int y )
{
	cout<<"x"<<x<<endl;
}
//Ĭ�ϲ���
void main1501()
{
	
	myPrint15(4);
	myPrint15();

	//
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//����ռλ���� ����������,����д������

void func1_15(int a, int b, int)
{
	cout<<"a"<<a<<" b"<<b<<endl;
}
//ռλ����
void main1502()
{
	
	//func1(1, 2); //err���ò�����
	func1_15(1, 2, 3);

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//Ĭ�ϲ�����ռλ����

void  func2_15(int a, int b, int =0)
{
	cout<<"a="<<a<<";b="<<b<<endl;
}

void main1503()
{
	func2_15(1, 2); //0k
	func2_15(1, 2, 3); //ok
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}