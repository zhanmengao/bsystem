#pragma once
#include <stdio.h>
#include<stack>
#ifdef _WIN32
#include<Windows.h>
#pragma comment(lib,"Ws2_32.lib")  
#else

#endif

void Swap(int* a, int* b)
{
	*a = *a^*b;
	*b = *b^*a;
	*a = *b^ *a;
	printf("a = %d b = %d\n", *a, *b);

	*a = *a^*b;
	*b = *b^*a;
	*a = *b^ *a;
	printf("a = %d b = %d\n", *a, *b);
}
int except(int num, int ex)
{
	return num >> (ex / 2);
}

void PrintfBinary(short num)
{
	short flag = 1;
	printf("%d(short):		", num);
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
void PrintfBinary(unsigned short num)
{
	unsigned short flag = 1;
	printf("%d(ushort):		", num);
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

void PrintfBinary(int num)
{
	int flag = 1;
	printf("%d(int):		", num);
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
void PrintfBinary(unsigned num)
{
	unsigned flag = 1;
	printf("%u(uint):		", num);
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
void PrintfBinary(unsigned long num)
{
	unsigned long flag = 1;
	printf("%u(ulong):		", num);
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
#ifdef __cplusplus
void PrintfBinaryRevert(short num)
{
	short flag = 1;
	std::stack<unsigned> mStack;
	while (flag)
	{
		if (num & flag)
		{
			mStack.push(1);
		}
		else
		{
			mStack.push(0);
		}
		flag = flag << 1;
	}
	printf("%d(short):		", num);
	while (!mStack.empty())
	{
		printf("%u", mStack.top());
		mStack.pop();
	}
	printf("\n");
}
void PrintfBinaryRevert(int num)
{
	unsigned flag = 1;
	std::stack<unsigned> mStack;
	while (flag)
	{
		if (num & flag)
		{
			mStack.push(1);
		}
		else
		{
			mStack.push(0);
		}
		flag = flag << 1;
	}
	printf("%d(int):		", num);
	while (!mStack.empty())
	{
		printf("%u", mStack.top());
		mStack.pop();
	}
	printf("\n");
}
void PrintfBinaryRevert(unsigned num)
{
	unsigned flag = 1;
	std::stack<unsigned> mStack;
	while (flag)
	{
		if (num & flag)
		{
			mStack.push(1);
		}
		else
		{
			mStack.push(0);
		}
		flag = flag << 1;
	}
	printf("%u(uin):	", num);
	while (!mStack.empty())
	{
		printf("%u", mStack.top());
		mStack.pop();
	}
	printf("\n");
}
void PrintfBinary(float f)
{
	int flag = 1;
	printf("%f(float):		", f);
	unsigned int* pa = (unsigned int*)(&f);
	for (int i = 31; i >= 0; i--)
		cout << (*pa >> i & 0x01) << (i == 31 || i == 23 ? "" : "");
	printf("\n");
}
#endif

void left(int* num, unsigned n)
{
	printf("Before Left:");
	PrintfBinary(*num);
	*num = *num << n;
	printf("After Left:");
	PrintfBinary(*num);
}

void right(int* num, unsigned n)
{
	printf("Before Right:");
	PrintfBinary(*num);
	*num = *num >> n;
	printf("After Right:");
	PrintfBinary(*num);
}

void right(unsigned* num, unsigned n)
{
	printf("Before Right:");
	PrintfBinary(*num);
	*num = *num >> n;
	printf("After Right:");
	PrintfBinary(*num);
}

int GetBit(unsigned flag, unsigned nBitsIdx)
{
	return ((flag & (1 << nBitsIdx)) > 0);
}
void SetBit(unsigned* flag, unsigned nBitsIdx, int val)
{
	if (val)
	{
		*flag |= 1 << nBitsIdx;
	}
	else
	{
		*flag &= (~(1 << nBitsIdx));
	}
}
void AddBits(unsigned* flag, int val)
{
	*flag |= val;
}
void AddBits64(unsigned long* flag, int val)
{
	*flag |= val;
}
void RemoveBits(unsigned* flag, int val)
{
    *flag &= (~val);							//和全1，仅指定位为0 做位与
}
void RemoveBits64(unsigned long* flag, long val)
{
	*flag &= (~val);
}
unsigned CountOf1(int n)
{
	unsigned count = 0;
	unsigned flag = 1;
	while (flag)
	{
		if (n & flag)
		{
			count++;
		}
		flag = flag << 1;
	}
	return count;
}
