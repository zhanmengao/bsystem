#pragma once
#include<iostream>
#include"Typedef.h"
using namespace std;


typedef struct test1
{
	char c;
	short s;
	int i;
	void* ptr;
};//12字节 内存对齐，1+2+4+4 =11，4字节向12对齐
class test2
{
public:
	 test2()
	{
	}
private:
	void Call()
	{
	}
};
class test3
{
public:
	test3()
	{
	}
	virtual ~test3()
	{
	}
private:
	void Call()
	{
	}
};//4 带vptr指针
enum test4
{
	TEST,
};
typedef struct
{
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
} sByte;				//1个字节
typedef struct
{
	unsigned char bit0 : 1;
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
	unsigned char bit8 : 1;
	unsigned char bit9 : 1;
	unsigned char bit10 : 1;
	unsigned char bit11 : 1;
	unsigned char bit12 : 1;
	unsigned char bit13 : 1;
	unsigned char bit14 : 1;
	unsigned char bit15 : 1;
} tShort;				//2个字节

typedef union
{
	unsigned char byte;
	sByte  bit;
}uByte8;			//一个字节，联合体大小取成员里最大的
typedef union
{
	unsigned short sint;
	tShort  bit;
}uByte16;			//2个字节，联合体大小取成员里最大的

static void SizeOf(MyArrayType arr)
{
	printf("In Function sizeof(arr): %u \n", sizeof(arr));
}
static void SizeOf(pArr pArr)
{
	printf("In Function sizeof(pArr): %u \n", sizeof(pArr));
}
static void mainSizeof()
{
	printf("sizeof(test1): %u \n", sizeof(struct test1));
	printf("sizeof(test2): %u \n", sizeof(struct test2));
	printf("sizeof(test3): %u \n", sizeof(struct test3));
	printf("sizeof(MyArrayType): %u \n", sizeof(MyArrayType));
	MyArrayType arr = { 1,2,3,4,5 };
	SizeOf(arr);
	printf("sizeof(pArr): %u \n", sizeof(pArr));
	SizeOf(&arr);
	printf("sizeof(test4): %u \n", sizeof(test4));
	printf("sizeof(sByte): %u \n", sizeof(sByte));
	printf("sizeof(uByte8): %u \n", sizeof(uByte8));
	cout << "sizeof(bool):" << sizeof(bool) << endl;
	cout << "sizeof(char):" << sizeof(char) << endl;
	cout << "sizeof(short):" << sizeof(short) << endl;
	cout << "sizeof(int):" << sizeof(int) << endl;
	cout << "sizeof(long):" << sizeof(long) << endl;
	cout << "sizeof(long long):" << sizeof(long long) << endl;
	cout << "sizeof(unsigned int):" << sizeof(unsigned int) << endl;
	cout << "sizeof(float):" << sizeof(float) << endl;
	cout << "sizeof(double):" << sizeof(double) << endl;
	void* pointer;
	cout << "sizeof(pointer):" << sizeof(pointer) << endl;
}