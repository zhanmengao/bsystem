#pragma once
#include"ByteUtils.h"
#include"BitUtils.h"
#include<stdint.h>
#include<iostream>

int main()
{
#ifdef _WIN32
    printf("逻辑与（&）：位都为1才是1\n");
	printf("0&0 = %d		1&0 = %d		0&1 = %d		1&1 = %d \n",
		0 & 0, 1 & 0, 0 & 1, 1 & 1);
    printf("逻辑或（|）：位有一个是1就是1\n");
	printf("0|0 = %d		1|0 = %d		0|1 = %d		1|1 = %d \n",
		0 | 0, 1 | 0, 0 | 1, 1 | 1);
	printf("逻辑异或（^）：两个位不一样就是1，两个位一样就是0\n");
	printf("0^0 = %d		1^0 = %d		0^1 = %d		1^1 = %d \n",
        0 ^ 0, 1 ^ 0, 0 ^ 1,  1^ 1);
#endif
	int val = 20;
	PrintfBinary(val);
	int a = 6;
	int b = 9;
	int aa = -a;
	unsigned u1 = 6;
	unsigned u2 = 9;
	right(&a,4);
	right(&u2, 4);
	right(&aa, 4);
	PrintfBinary(INT32_MAX);
	PrintfBinary(INT32_MIN);
	PrintfBinary(UINT32_MAX);
	PrintfBinary((unsigned)0);

	{
			short x = 12345;						//0x3039
			short mx = -x;								//0xcfc7
			int y = 0x01234567;
			PrintBytes((byte_pointer)&x, sizeof(x));
			PrintfBinaryRevert(x);
			PrintBytes((byte_pointer)&mx, sizeof(mx));
			PrintfBinaryRevert(mx);
			PrintBytes((byte_pointer)&y, sizeof(y));
			PrintfBinary(y);
			PrintfBinaryRevert(y);
			int z = 0x87654321;
			PrintBytes((byte_pointer)&z,1);
			PrintBytes((byte_pointer)&z,2);
			PrintBytes((byte_pointer)&z,3);
	}
	{
			int o = 3510593;
			float of = 3510593.0f;
			PrintBytes((byte_pointer)&o,sizeof(o));	
			PrintfBinary(o);
			PrintBytes((byte_pointer)&of,sizeof(of));	
			//PrintfBinary(of);
			int i = -1;
			PrintfBinary(i);
			right(&i,1);
			int j = 1;
			PrintfBinary(j);
			right(&j,1);
	}

	{
		PrintfBinary(float(0.0f));
		PrintfBinary(float(1.0f/512));
		PrintfBinary(float(7.0f / 512));
		PrintfBinary(float(1.0f/64));
		PrintfBinary(float(7.0f / 8));
		PrintfBinary(float(1.0f));
		PrintfBinary(float(1.125f));
		PrintfBinary(float(240.0f));
	}
	
	//Convert
	

	Swap(&a, &b);
	printf("except(a, 2) = %d \n", except(a, 2));
	PrintfBinary(a);
	printf("CountOf1(a) = %d \n", CountOf1(a));

	left(&b, 1);
	right(&b, 1);

	unsigned int c = 0;
	SetBit(&c, 0, 1);
    std::cout << "GetBit(0) : " << GetBit(c, 0) << std::endl;
	SetBit(&c, 9, 1);
	PrintfBinary(c);

	{
		uint8_t x = 1 << 1 | 1 << 5;
		uint8_t y = 1 << 1 | 1 << 2;

		PrintfBinary<uint8_t>(x);
		PrintfBinary<uint8_t>(y);
		PrintfBinary<uint8_t>(x & y);
		PrintfBinary<uint8_t>(x | y);
		PrintfBinary<uint8_t>(x ^ y);
	}
    return 0;
}
