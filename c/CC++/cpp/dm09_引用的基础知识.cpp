
#include <iostream>
using namespace std;

//1 ���õĸ��� 
//2 ����C++��������C����չ,������c���Ե��﷨ȥ˼����
void main91()
{
	int  a = 10;

	//���õ��﷨��Type& name = var;
	int &b = a;

	b = 100; //�൱�ڰ�a�޸ĳ�100��.

	printf("b:%d \n", b);
	printf("a:%d \n", a);

	a = 200;
	printf("b:%d \n", b);
	printf("a:%d \n", a);

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}

void  main92()
{
	int a = 10;
	int &b = a;
	//int &c ; //03��ͨ���� ����Ҫ��ʼ��
	system("pause");
}

//�������͵����� 

void myswap(int a, int b)
{
	int c = 0;
	c = a;
	a = b;
	b = c;
}

void myswap02(int *a, int *b)
{
	int c = 0;
	c = *a;
	*a = *b;
	*b = c;
}

//04 ������Ϊ������������ʱ�����г�ʼ��
void myswap03(int &a, int &b)
{
	int c = 0;
	c = a;
	a = b;
	b = c;
}
void  main93()
{
	int x, y ;
	x = 10;
	y = 20;

	myswap(x, y);
	printf("x:%d , y:%d \n", x, y);

	myswap02(&x, &y);
	printf("x:%d , y:%d \n", x, y);

	//a����x�ı���  b����y�ı���
	myswap03(x, y);
	printf("x:%d , y:%d \n", x, y);

	//int &c ; //��ͨ���� ����Ҫ��ʼ��
	system("pause");
}

//05������������ ������
struct Teacher
{
	char name[64];
	int age ;
};

void printfT(Teacher *pT)
{
	cout<<pT->age<<endl;
}

//pT��t1�ı��� ,�൱���޸���t1
void printfT2(Teacher &pT)
{
	//cout<<pT.age<<endl;
	pT.age = 33;
}

//pT��t1����������ͬ�ı���
void printfT3(Teacher pT)
{
	cout<<pT.age<<endl;
	pT.age = 45; //ֻ���޸�pT���� ,�����޸�t1����
}
void main008978()
{
	Teacher t1;
	t1.age = 35;

	printfT(&t1);

	printfT2(t1); //pT��t1�ı���
	printf("t1.age:%d \n", t1.age); //33

	printfT3(t1) ;// pT���β� ,t1 copyһ������ ��pT     //---> pT = t1
	printf("t1.age:%d \n", t1.age); //35
	
	cout<<"hello..."<<endl;
	system("pause");
	return ;
}