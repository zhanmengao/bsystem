#pragma once
#include <stdio.h>
#include<stdint.h>


typedef unsigned char *byte_pointer;
void PrintBytes(byte_pointer start, size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		printf(" %.2x", start[i]);
	}
	printf("\n");
}

void PrintInt(short x)
{
	PrintBytes((byte_pointer)&x, sizeof(x));
}
void PrintInt(int x)
{
	PrintBytes((byte_pointer)&x, sizeof(x));
}
void PrintUInt(unsigned x)
{
	PrintBytes((byte_pointer)&x, sizeof(x));
}
void Printfloat(float x)
{
	PrintBytes((byte_pointer)&x, sizeof(x));
}
void PrintPointer(void* x)
{
	PrintBytes((byte_pointer)&x, sizeof(x));
}

int uadd_ok(unsigned x,unsigned y)
{
	unsigned sum = x + y;
	return sum>=x && sum >= y;
}

uint16_t ByteSwap2(uint16_t inData)
{
	return (inData >> 8) | (inData << 8);
}
uint32_t ByteSwap4(uint32_t inData)
{
	return	((inData >> 24) &  0x0000000ff)		|
				((inData >> 8) & 0x0000ff00)			|
				((inData << 8) & 0x00ff0000)			|
				((inData << 24) & 0xff00000);
}
uint64_t ByteSwap8(uint64_t inData)
{
	return	((inData >> 56) & 0x00000000000000ff) |
				((inData >> 40) & 0x000000000000ff00) |
				((inData >> 24) & 0x0000000000ff0000) |
				((inData >> 8) & 0x00000000ff000000) |
				((inData << 8) & 0x000000ff00000000) |
				((inData << 24) & 0x0000ff0000000000) |
				((inData << 40) & 0x00ff000000000000) | 
				((inData << 56) & 0xff00000000000000);

}

#ifdef __cplusplus
template<size_t tSize>
void PrintBytes(byte_pointer start)
{
	PrintBytes(start, tSize);
}
template<typename T>
void PrintBytes(T num)
{
	PrintBytes(&num, sizeof(T));
}
template<typename tFrom, typename tTo>
class TypeAliaser
{
public:
	TypeAliaser(tFrom inFromValue) :mAsFromType(inFromValue)
	{

	}
	tTo& Get() { return mAsToType; }
private:
	union
	{
		tFrom mAsFromType;
		tTo mAsToType;
	};
};

template<typename T, size_t tSize> class ByteSwapper;			//非类型模板参数

template<typename T>
class ByteSwapper<T, 2>
{
public:
	T Swap(T inData) const
	{
		uint16_t result = ByteSwap2(TypeAliaser<T, uint16_t>(inData).Get());
		return TypeAliaser<uint16_t, T>(result).Get();
	}
};

template<typename T>
class ByteSwapper<T, 4>
{
public:
	T Swap(T inData) const
	{
		uint32_t result = ByteSwap4(TypeAliaser<T, uint32_t>(inData).Get());
		return TypeAliaser<uint32_t, T>(result).Get();
	}
};

template<typename T>
class ByteSwapper<T, 8>
{
public:
	T Swap(T inData) const
	{
		uint64_t result = ByteSwap8(TypeAliaser<T, uint64_t>(inData).Get());
		return TypeAliaser<uint64_t, T>(result).Get();
	}
};

template<typename T>
T ByteSwap(T inData)
{
	return ByteSwapper<T, sizeof(T)>().Swap(inData);
}
#endif
