package com.SortSeek;

public class CountSort {
	public CountSort(int array[],int min,int max)
	{
		//B数组存储排序后的输出
		int B[]=new int[array.length];
		//c数组记录区间[min,max]内每个数出现的频率
		int C[]=new int[max-min];
		
		//初始化C
		for(int i=0;i<C.length;i++)
		{
			C[i]=0;
		}
		//统计每个数出现频数，记录到C数组
		for(int j=0;j<array.length;j++)
		{
			C[array[j]]++;
		}		
		for(int j=0;j<C.length;j++)
		{
			//自底向上动态规划：统计有多少数小于当前
			C[array[j]]+=C[array[j]-1];
		}
		//C数组此时已经存储了 c[j]应该放在第几个位置：j为它的值，C[j]为小于等于它的数的个数->j的正确摆放位置
		for(int j=B.length-1;j>=0;j--)
		{
			
			B[C[j]]=array[j];
			C[j]--;
		}
		array=B;
	}
}





