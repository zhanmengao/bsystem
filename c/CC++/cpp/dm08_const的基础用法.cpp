
#include <iostream>
using namespace std;


//0 const�Ļ���֪ʶ
struct Teacher
{
	char name[64];
	int age;
};

//ָ����ָ����ڴ�ռ�,���ܱ��޸�
int  operatorTeacher01(const Teacher *pT)
{
	//pT->age = 10;
	return 0;
}

//ָ����������ܱ��޸�
int  operatorTeacher02( Teacher * const pT)
{
	pT->age = 10;
	//pT = NULL; //
	return 0;
}

int  operatorTeacher03( const Teacher * const pT)
{
	//pT->age = 10;
	//pT = NULL; //
	printf("age:%d", pT->age);
	return 0;
}


void main81()
{
// 	const int a;
// 	int const b; //һ��
// 
// 	const int *c;  //const���ε���ָ����ָ����ڴ�ռ�,���ܱ��޸�
// 	int * const d; //
// 	const int * const e ;
	cout<<"hello..."<<endl;


	Teacher t1;
	t1.age = 33;
	operatorTeacher03(&t1);

	system("pause");
	return ;
}

//1
//c�����е�const��һ��ð�ƻ�
//C++������ const��һ�������ĳ��� 

//2 ԭ�����
//const int b = 10; ���ű�

void main82()
{
	//���� a ��һ������
	const int a = 10;
	//a = 11;
	int *p = NULL;
	p = (int *)&a;

	*p = 20;  //��Ӹ�ֵ 

	printf("a:%d \n", a);
	printf("*p:%d \n", *p);
	system("pause");
}

//3 const�����ڴ��ʱ�� �������������ڼ� 
void main83()
{

	//���� a ��һ������
	int a;
	const int b = 10;
	int c; 

	printf("&a:%d, &b:%d, &c:%d \n", &a, &b, &c);
	
	system("pause");
	


}

//4const �� #define����֮ͬ��
//#define, �ڱ���Ԥ����׶� ����
//const�������ɱ���������ģ��ṩ���ͼ�����������
#define d  20
void main84()
{
	//int a = 10;
	//int b = 20;
	//int array[a+b]; //linux�ں������ǳ�����;ԭ�� ����linux�ں˵�gcc������֧��.
					//c��c++������ ��֧�������﷨���� 

	const int c = 10;
	//const int d = 20;
	int array2[c+d]; 

	system("pause");

}

//5 const����ı���,�ɱ���������ģ��ṩ���ͼ�����������
void fun1()
{
	#define a 10
	const int b = 20;
	//#undef  a 
	//# undef
}

void fun2()
{
	printf("a = %d\n", a);
	//printf("b = %d\n", b);
}

int main1111()
{
	fun1();
	fun2();
	return 0;
}




