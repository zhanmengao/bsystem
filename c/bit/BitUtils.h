#pragma once
#include <stdio.h>
#ifdef __cplusplus
#include<stack>
#endif
#ifdef _WIN32
#include<Windows.h>
#pragma comment(lib,"Ws2_32.lib")  
#else

#endif

void Swap(int* a, int* b);

int except(int num, int ex);

template<typename T>
void PrintfBinary(T num)
{
	T flag = 1;
	while (flag)
	{
		if (num & flag)
		{
			printf("%u", 1);
		}
		else
		{
			printf("%u", 0);
		}
		flag = flag << 1;
	}
	printf("\n");
}
void PrintfBinary(short num);
void PrintfBinary(unsigned short num);

void PrintfBinary(int num);
void PrintfBinary(unsigned num);
void PrintfBinary(unsigned long num);

#ifdef __cplusplus
void PrintfBinaryRevert(short num);
void PrintfBinaryRevert(int num);
void PrintfBinaryRevert(unsigned num);
void PrintfBinary(float f);
#endif

void left(int* num, unsigned n);

void right(int* num, unsigned n);

void right(unsigned* num, unsigned n);

int GetBit(unsigned flag, unsigned nBitsIdx);
void SetBit(unsigned* flag, unsigned nBitsIdx, int val);
void AddBits(unsigned* flag, int val);
void AddBits64(unsigned long* flag, int val);
void RemoveBits(unsigned* flag, int val);
void RemoveBits64(unsigned long* flag, long val);
unsigned CountOf1(int n);
bool isBigEndian();
