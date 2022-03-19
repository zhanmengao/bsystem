#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//一旦结构体定义下来,则结构体中的成员..内存布局 就定下了
// 可以通过 age地址 .去 求 大的结构体的内存地址
typedef struct AdvTeacher
{
	char name[64]; //64
	int age ;  //4
	int p; //4
	char *pname2; //4

}AdvTeacher;


void main61()
{
	int i = 0;
	AdvTeacher  t1;
	AdvTeacher *p = NULL;
	//strcpy(0, "dddd");
	p - 1 ; //编译通过 运行通过 这句话 在cpu中计算
	p - 2;
	p - 3;

	p - p;

	{
		int offsize = (int)&(p->age);
		printf("%d \n", offsize);
	}
}


void mainOffset()
{
	int i = 0;
	AdvTeacher  t1; // & (t1.age)
	AdvTeacher *p = NULL;
	p = &t1;
	//p = p + 100;
	//strcpy(0, "dddd");
	//p - 1 ; //编译通过 运行通过 这句话 在cpu中计算
	//p - 2;
	//p - 3;

	
	{
		int offsize1 =   (int)&(p->age) - (int)p; // & (t1.age)
		int offsize2 = (int )&(((AdvTeacher *)0)->age );
		printf("offsize1:%d \n", offsize1);
		printf("offsize2:%d \n", offsize2);

	}
}