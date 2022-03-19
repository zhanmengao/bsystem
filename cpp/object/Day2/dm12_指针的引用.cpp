#include "iostream"
using namespace std;

//
struct Teacher
{
	char name[64];
	int age ;
};

//在被调用函数 获取资源 
int getTeacher(Teacher **p)
{
	Teacher *tmp = NULL;
	if (p == NULL)
	{
		return -1;
	}
	tmp = (Teacher *)malloc(sizeof(Teacher));
	if (tmp == NULL)
	{
		return -2;
	}
	tmp->age = 33;
	// p是实参的地址  *实参的地址 去间接的修改实参的值。*p存储的是堆空间的地址
	*p = tmp; 
	return 0;
}


//指针的引用 做函数参数  myp是一个指针的别名
int getTeacher2(Teacher* &myp)
{
	//给myp赋值 相当于给main函数中的pT1赋值
	myp = (Teacher *)malloc(sizeof(Teacher));
	if (myp == NULL)
	{
		return -1;
	}
	myp->age = 36;
	return 0;
}

void FreeTeacher(Teacher *pT1)
{
	if (pT1 == NULL)
	{
		return ;
	}
	free(pT1);
	pT1 = nullptr;
}

void main1201()
{
	Teacher *pT1 = NULL;

	//1 c语言中的二级指针
	getTeacher(&pT1);
	cout<<"age:"<<pT1->age<<endl;
	FreeTeacher(pT1);


	//2 c++中的引用 （指针的引用）
	//引用的本质 间接赋值后2个条件 让c++编译器帮我们程序员做了。
	getTeacher2(pT1);

	cout<<"age:"<<pT1->age<<endl;
	FreeTeacher(pT1);

	system("pause");
}

