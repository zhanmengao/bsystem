//
// Created by 25123 on 2020/10/17.
//

#ifndef STB_BITMAP_H
#define STB_BITMAP_H
#include"BitUtils.h"

#ifdef __cplusplus
class BitMap32
{
public:
	void SetBit(int nBitsIdx, int val)
	{
		::SetBit(&mData, nBitsIdx, val);
	}
	int GetBit(int nBitsIdx)
	{
		return ::GetBit(mData, nBitsIdx);
	}
	unsigned GetData() const
	{
		return mData;
	}
	unsigned& GetData()
	{
		return mData;
	}
private:
	unsigned mData = 0;
};


void mainBitmap()
{
	BitMap32 bits;
	bits.SetBit(31, 1);
	AddBits((unsigned*)&bits, 4);
	printf("%d \n",bits.GetBit(31));
	PrintfBinary(bits.GetData());
	//RemoveBits(&bits.GetData(), 4);
	PrintfBinary(bits.GetData());
}
#endif //STB_BITMAP_H
#endif
