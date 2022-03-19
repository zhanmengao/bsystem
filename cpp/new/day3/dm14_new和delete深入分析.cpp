
#include <iostream>
using namespace std;

// 1
//		malloc	free				c语言的函数
//		new		delete 操作符	c++的语法

//2		new 基础类型变量  分配数组变量  分配类对象

//3		

////分配基础类型
void main1401()
{
	// 
	int *p = (int *)malloc(sizeof(int));
	*p = 10;
	//free(p);
	delete p;

	int *p2 = new int; //分配基础类型
	*p2 = 20;
	free(p2);
	//
	int *p3 = new int(30);
	printf("*p3:%d \n", *p3);
	//delete p3;
	free(p3);

	cout << "hello..." << endl;
	system("pause");
	return;
}

//分配数组变量
void main1402()
{
	//c语言分配数组
	int *p = (int *)malloc(sizeof(int) * 10);  //int array[10];
	p[0] = 1;
	//free(p);
	delete[] p;

	//c++分配数组 
	int *pArray = new int[10];
	pArray[1] = 2;
	//delete [] pArray; //数组不要把[] 忘记
	free(pArray);

	char *pArray2 = new char[25]; //char buf[25]
	delete[] pArray2;


	cout << "hello..." << endl;
	system("pause");
	return;
}

class Test14
{
public:
	Test14(int _a)
	{
		a = _a;
		cout << "构造函数执行" << endl;
	}

	~Test14()
	{
		cout << "析构函数执行" << endl;
	}

protected:
private:
	int a;
};

//分配对象new delete
//相同 和 不同的地方 new能执行类型构造函数   delete操作符 能执行类的析构函数

//	malloc 	free函数 C
//1 new 	delete 操作符号 c++的关键字
//结论
void main1403()
{
	//c 
	Test14 *pT1 = (Test14 *)malloc(sizeof(Test14));
	free(pT1);//不执行析构函数
	//delete pT1; 

	//c++
	Test14 *pT2 = new Test14(10);
	delete pT2;
	//free(pT2);
}