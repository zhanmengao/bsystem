
#include <iostream>
using namespace std;

int getAA1()
{
	int a ;
	a = 10;
	return a;
}

//����a�ı��� ����a��һ������ 10 
int& getAA2()
{
	int a ; //�������ջ�ϵ� ����, �п��ܻ�������
	a = 10;
	return a;
}

int* getAA3()
{
	int a ;
	a = 10;
	return &a;
}

void main1101()
{
	int a1 = 0;
	int a2 = 0;
	a1 = getAA1();

	a2 = getAA2(); //10
	int &a3 = getAA2();  //������ջ����	���ܳ�Ϊ�������õĳ�ʼֵ

	printf("a1:%d \n", a1);
	printf("a2:%d \n", a2); 
	printf("a3:%d \n", a3);  // *a3

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//������static ������ ȫ�ֱ���

int j1()
{
	static int a = 10;
	a ++ ;
	return a;
}

int& j2()
{
	static int a = 10;
	a ++ ;
	return a;
}

//�����ؾ�̬������ȫ�ֱ���
//	���Գ�Ϊ�������õĳ�ʼֵ
//	������Ϊ��ֵʹ�ã�Ҳ����Ϊ��ֵʹ��
void main1102()
{
	int a1 = 10;
	int a2 = 20;

	a1 = j1();
	a2 = j2();
	int &a3 = j2();


	printf("a1:%d \n", a1);
	printf("a2:%d \n", a2); 
	printf("a3:%d \n", a3);  
	system("pause");
}


//////////////////////////////////////////////////////////////////////////
//--- ��������ֵ
//���ر�����ֵ
int g1()
{
	static int a = 10;
	a ++ ;
	return a;  //
} 

//���ر������� , 
int& g2()
{
	static int a = 10;
	a ++ ;
	printf("a:%d \n" , a);
	return a;
}

void main3213123()
{
	// g1() = 100;
	//11 = 100;

	g2() = 100; //��������ֵ��һ������,���ҵ���ֵ 
	g2();

	int c1 = g1(); //��������ֵ��һ������,���ҵ���ֵ 
	int c2 = g2(); //��������ֵ��һ������,���ҵ���ֵ 
	//a = 100;
	system("pause");
}