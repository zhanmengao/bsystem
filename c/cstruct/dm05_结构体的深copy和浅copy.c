#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Teacher5
{
	char name[64];
	int age ;
	char *pname2;
	
}Teacher5;


//编译器的=号操作,只会把指针变量的值,从from  copy 到 to,但 
//不会 把指针变量 所指 的 内存空间 给copy过去..//浅copy

//结构体中套一个 1级指针 或 二级指针 

//如果 想执行深copy,再显示的分配内存
void copyTeacher51(Teacher5 *to, Teacher5 *from)
{
	*to = *from;

	to->pname2 = (char *)malloc(100);
	strcpy(to->pname2, from->pname2);

	//memcpy(to, from , sizeof(Teacher));
}
void main51a()
{
	Teacher5 t1;
	Teacher5 t2;

	strcpy(t1.name, "name1");
	t1.pname2 = (char *)malloc(100);
	strcpy(t1.pname2, "ssss");

	//t1 copy t2

	copyTeacher51(&t2, &t1);          //执行深拷贝

	if (t1.pname2 != NULL)
	{
		free(t1.pname2);
		t1.pname2 = NULL;
	}

	if (t2.pname2 != NULL)
	{
		free(t2.pname2);
		t2.pname2 = NULL;
	}
}