#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//定义一个结构体类型

//定义了一个数据类型 。固定大小内存块的别名 还没有分配内存
//类型的重定义

typedef struct Teacher
{
	char name[64];
	int age ;
	int id;
}Teacher;


//用类型定义变量的方法3种

struct Student
{
	char name[64];
	int aga ;
}s1, s2 ;  //定义类型的同时 定义变量

struct 
{
	char name[64];
	int aga ;
}s3, s4;  //匿名类型 定义变量

//初始化变量的三种方法
//定义变量 ，然后初始化
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
	//struct Teacher  t1; //告诉C编译器给我分配内存

	Teacher  t1; //告诉C编译器给我分配内存//1
	Teacher  t2 = {"aaaa", 32, 02};
	printf("sizeof(t1): %u \n", sizeof(t1));
	t1.age = 31;  //t1. 操作符  是 干什么  有没有操作内存
					//是寻址操作 计算age 相对于 t1大变量的 偏移量 ===》计算 cup中进行。。
					//没有操作内存

	//通过指针的方式 操作 内存空间
	{
		Teacher *p = NULL;
		p = &t2;
		printf("p->age:%d \n", p->age); //->  是寻址操作 算age 相对于 t2大变量的  偏移量  计算 cup中进行。
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

	t2 = t1; //=号操作下 编译器的行为

	//编译器给我们提供 简单 =号 赋值操作 。。。我们要顺从
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