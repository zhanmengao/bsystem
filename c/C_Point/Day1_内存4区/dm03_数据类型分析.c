#include "stdlib.h"
#include "stdio.h"
#include "string.h"

// �������͵���;
//�������͵ı���:�̶���С�ڴ��ı���
// b &b ������������ (����һ��1 �������� 2����ָ��  3 �������ͺ�����ָ�����͵Ĺ�ϵ) ====>ѹ����ѧ�ߵ�������ɽ  ��ש
//

void main104()
{
	int a;                     //����c����������4���ֽڵ��ڴ�
	int b[10] ;               //����c����������40���ֽ��ڴ�

	printf("b = %u, b+1 = %u, &b = %u, &b+1 = %u \n", b, b+1, &b, (&b)+1);                 //&b=����Ԫ���׵�ַ    (&b)=�������int[10]���ڴ�ռ�

	printf("sizeof(b):%d \n", sizeof(b));  //40 ->  b=int[10]
	printf("sizeof(a):%d \n ", sizeof(a)); //4
	 
	// b+1  &b+1 �����һ��  //b &b��������������Ͳ�һ��
	//b �����������Ԫ�صĵ�ַ
	//&b���������������ĵ�ַ  

}

struct Teacher
{
	char name[64];
	int age;
}Teacher;


typedef struct Teacher2
{
	char name[64];
	int age;
}Teacher2;
//���ݱ��� typedef

typedef int u32;


void main105()
{
	int a; //����c����������4���ֽڵ��ڴ�
	int b[10] ; //����c����������40���Լ��ڴ�

	struct Teacher t1;

	Teacher2 t2;
	t1.age = 31;

	printf("u32:%d \n", sizeof(u32));

	{
		char *p2 = NULL;
		void *p1 = NULL;
		p2 = (char *)malloc(100);

		p1 = &p2;
	}
	{
		//void a;//��������֪����η����ڴ�
	}

}