#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//����һ���ṹ������

//������һ���������� ���̶���С�ڴ��ı��� ��û�з����ڴ�
//���͵��ض���

typedef struct Teacher
{
	char name[64];
	int age ;
	int id;
}Teacher;


//�����Ͷ�������ķ���3��

struct Student
{
	char name[64];
	int aga ;
}s1, s2 ;  //�������͵�ͬʱ �������

struct 
{
	char name[64];
	int aga ;
}s3, s4;  //�������� �������

//��ʼ�����������ַ���
//������� ��Ȼ���ʼ��
Teacher  t3 = {"aaaa", 31, 01};
struct Student2
{
	char name[64];
	int aga ;
}s5 = {"names", 21};

struct 
{
	char name[64];
	int aga ;
}s6 = {"names", 21};


void main501()
{
	//struct Teacher  t1; //����C���������ҷ����ڴ�

	Teacher  t1; //����C���������ҷ����ڴ�//1
	Teacher  t2 = {"aaaa", 32, 02};
	printf("sizeof(t1): %u \n", sizeof(t1));
	t1.age = 31;  //t1. ������  �� ��ʲô  ��û�в����ڴ�
					//��Ѱַ���� ����age ����� t1������� ƫ���� ===������ cup�н��С���
					//û�в����ڴ�

	//ͨ��ָ��ķ�ʽ ���� �ڴ�ռ�
	{
		Teacher *p = NULL;
		p = &t2;
		printf("p->age:%d \n", p->age); //->  ��Ѱַ���� ��age ����� t2�������  ƫ����  ���� cup�н��С�
		printf("p->name:%s \n", p->name);
	}
	strcpy(t1.name, "t1name");

	printf("t1.name:%s \n", t1.name);
}

void  copyTeacher11(Teacher to, Teacher from )
{
	to = from;	
}

void  copyTeacher12(Teacher *to, Teacher *from )
{
	//(*to) = (*from);
	*to = *from;
}

void main111()
{

	Teacher  t1 = { "aaaa", 32, 02 };
	Teacher  t2;
	Teacher  t3;
	memset(&t3, 0, sizeof(t3));

	t2 = t1; //=�Ų����� ����������Ϊ

	//�������������ṩ �� =�� ��ֵ���� ����������Ҫ˳��
	printf("t2.name:%s \n", t2.name);
	printf("t2.age:%d \n", t2.age);

	copyTeacher11(t3, t1);
	printf("copyTeacher() after \n");
	printf("t3.name:%s \n", t3.name);
	printf("t3.age:%d \n", t3.age);
	printf("hello...\n");


	copyTeacher12(&t3, &t1);
	printf("copyTeacher02() after \n");
	printf("t3.name:%s \n", t3.name);
	printf("t3.age:%d \n", t3.age);
}