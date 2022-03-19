#pragma once
#include <iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

class Student
{
public:
    //默认构造函数，没有参数
	Student():age(20),num(1000),buf(nullptr),len(0)
	{
	};
	//初始化构造函数，有参数和参数列表
	Student(int a, int n) :age(a), num(n), buf(nullptr) {};

	//拷贝构造函数，这里与编译器生成的一致
	Student(const Student& s):age(s.age),num(s.num),buf(nullptr),len(0)
	{
		if (s.buf != nullptr)
		{
			buf = (char*)malloc(s.len);
			memcpy(buf, s.buf, sizeof(buf));
			len = s.len;
		}
	};

	//转移构造函数
	Student(Student&& s):age(s.age),num(s.num),buf(s.buf),len(s.len)
	{
		s.buf = nullptr;
		s.len = 0;
	}

    //转换构造函数,形参是其他类型变量，且只有一个形参
	Student(int r):age(r),num(1002),buf(nullptr),len(0)
	{

	};

	~Student()
	{
		if (buf != nullptr)
		{
			free(buf);
			buf = nullptr;
			len = 0;
		}
	}
public:
	int age;
	int num;
	char *buf;
	int len;
};

int main()
{
	Student s1;
	Student s2(18, 1001);
	int a = 10;
	Student s3(a);
	Student s4(s3);

	printf("s1 age:%d, num:%d\n", s1.age, s1.num);
	printf("s2 age:%d, num:%d\n", s2.age, s2.num);
	printf("s3 age:%d, num:%d\n", s3.age, s3.num);
	printf("s2 age:%d, num:%d\n", s4.age, s4.num);
	return 0;
}
//运行结果
//s1 age:20, num:1000
//s2 age:18, num:1001
//s3 age:10, num:1002
//s2 age:10, num:1002
