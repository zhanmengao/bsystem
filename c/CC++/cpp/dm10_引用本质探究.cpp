
#include <iostream>
using namespace std;

//1 ��һ�� �������������ʱ�������ʼ����˵������һ������
void main1001()
{
	//
	 const  int c1 = 10;

	 int a = 10;
	 int &b = a;  //b����һ������

	 printf("&a:%d \n", &a);
	 printf("&b:%d \n", &b); //===> a �� b����ͬһ���ڴ�ռ�����ƺ�
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

//2 ��ͨ�������Լ��Ŀռ��� �� 
struct Teacher
{
	char name[64]; //64
	int age; //4 

	int &a; //4 0  //����ָ�� ��ռ���ڴ�ռ��С
	int &b; //4 0 
};

//3 ���õı���

void modifyA(int &a1)
{
	a1 = 100;
}

void modifyA2(int * const a1)
{
	*a1 = 200;  //*ʵ�εĵ�ַ ,ȥ��ӵ��޸�ʵ�ε�ֵ 
}

void main1002()
{
	int a = 10;

	//1
	modifyA(a); //ָ������������õ�ʱ��,���ǳ���Ա����Ҫȡa�ĵ�ַ
	printf("a:%d \n", a);

	//
	a = 10;
	modifyA2(&a); //�����ָ�� ��Ҫ���ǳ���Ա�ֹ���ȡʵ�εĵ�ַ 
	printf("a:%d \n", a); 

	printf("sizeof(Teacher):%d \n", sizeof(Teacher));
	system("pause");
}

void modifyA3(int *p)
{
	*p = 200;  //*p 3*p�β�ȥ����޸�ʵ�ε�ֵ
}
//��Ӹ�ֵ
void main213131()
{
	int a = 10;
	int *p = NULL; //��Ӹ�ֵ�������������� 1 ������������ 

	p = &a;
	*p = 100;

	{
		*p = 200;
	}

	modifyA3(&a); //2 ��������

}


// 123 д��һ��
// 12   3
//1      23