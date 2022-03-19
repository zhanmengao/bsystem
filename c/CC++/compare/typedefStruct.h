#pragma once
//C语言里
#ifndef __cplusplus
typedef struct Student
{
	int age;
} S;
//等价于

// c
struct Student
{
	int age;
};
#endif
//C++里
#ifdef __cplusplus
//表现为可以使用 Student ，也可以使用 struct Student，如下：
// cpp
#if 1
{
	struct Student
	{
		int age;
	};
	void f(Student me);       // 正确，"struct" 关键字可省略
}
#endif

//.如果在类标识符空间定义了 struct Student {...};，使用 Student me; 时，
//编译器将搜索全局标识符表，Student 未找到，则在类标识符内搜索。
//若定义了与 Student 同名函数之后，则 Student 只代表函数，不代表结构体
typedef struct Student
{
	int age;
} S;

void Student() {}           // 正确，定义后 "Student" 只代表此函数

//void S() {}               // 错误，符号 "S" 已经被定义为一个 "struct Student" 的别名

int maintypedef()
{
	Student();
	struct Student me;      // 或者 "S me";
	return 0;
}
#endif