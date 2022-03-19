#include"ByteUtils.h"
#include"BitUtils.h"
#include<stdint.h>
#define INT32 2147483647
#define UINT32 2147483647U
int main()
{
	{	
		int a = INT32_MIN;
		unsigned ua = a;
		PrintfBinary(a);
		PrintInt(a);
		PrintfBinary(ua);
		PrintUInt(ua);
		if(ua == a-2^32)
		{
			printf("Convert int to uint , ua == a-2^32 \n");
		}
		unsigned ub = UINT32_MAX;
		int b = ub;
		PrintfBinary(ub);
		PrintUInt(ub);
		PrintfBinary(b);
		PrintInt(b);
		if(ub == b+2^32)
		{
			printf("Convert uint to int , ub == b+2^32 \n");
		}	
	}
	
	{
		printf(" -1<0 %d 	-1<0U %d\n",-1<0,-1<0U);
		printf(" -1>-2 %d 	(unsigned)-1>-2 %d\n",-1>2,(unsigned)-1>-2);	
		printf(" NUM32>-NUM21-1 %d 	UNUM21>-NUM21-1 %d	NUM32>(int)UNUM32 %d \n",INT32>-INT32-1,UINT32>-INT32-1,INT32>(int)UINT32);	
		int i = 0;
		unsigned size = 0;
		printf("i = %u size-1 = %u i<=size-1 %d \n",i,size-1,i<=size-1);
		
		unsigned size2 = 99999;
		printf("size - size2 = %u  \n",size-size2);
		
		printf("0 - 1 =  d  = %d   u = %u\n",0-1,0-1);
	}
	
	{
		short sx = -12345;
		unsigned short usx = sx;
		PrintfBinary(sx);
		PrintfBinary(usx);
		int x = sx;									//short -> int 	fill with 1
		PrintfBinary(x);	
		unsigned ux = x;
		PrintfBinary(ux);
		unsigned uix = usx;
		 PrintfBinary(uix);					//unsigned short -> unsigned fill with 0
		unsigned uy = sx;
		PrintfBinary(uy);					//short -> (int) -> unsigned
		
		PrintfBinary((int)((uy<<24)>>24));							//uint >> fill 0
		PrintfBinary(((int)uy<<24)>>24);							//int >> fill 1
	}
	return 0;
} 




