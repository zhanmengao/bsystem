
#include <iostream>
using namespace std;

//1 �����﷨ 
//2 �����쳣֮��,�ǿ纯�� :
//3 �����쳣�Ժ� ���Բ����� ���׳��쳣
//4 catch�쳣��ʱ �������ͽ���catch

//5 �쳣��׽�ϸ�������ƥ��

void main20111()
{
	try
	{
		throw 'Z';
	}
	catch (int e)
	{
		cout << "����int�����쳣" << endl;
	}
	catch(...)
	{
		cout << " δ֪ �����쳣" << endl;
	}
}


void divide(int x, int y)
{
	if (y == 0)
	{
		throw x; //�׳� int���� �쳣
	}

	cout << "divide���:" << x/y<< endl;
}

void main202(int x, int y)
{
	try
	{
		divide(x, y);
	}
	//�źŲ�׽����
	catch (...)
	{
		cout << "�ҽ����� divide���쳣 ������û�д��� �������׳�" << endl;
		throw ; //��������ϵͳִ���źŵ�Ĭ�ϴ�����
	}
}

void main203()
{
	try
	{
		//divide(10, 2);
		//divide(100, 0);

		divide(100, 0);
	}
	catch (int e)
	{
		cout << e << "�����" << endl;
	}
	catch ( ... )  //
	{
		cout <<  "����δ֪�����쳣 "<< endl;
	}
	
}