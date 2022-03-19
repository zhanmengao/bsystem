package com.SortSeek;

public class Select {
	//查找一个数组中指定元素的位置
	//输入：升序数组，指定元素，开始位置，结束位置 --以0为初始
	//输出：position
	public static int getPosition(int []asc,int value,int begin,int end)
	{
		if(begin>end || end>asc.length)//结束检测
		{
			System.out.println("没有找到");
			return Integer.MAX_VALUE;
		}
		int MidIndex=begin+(end-begin)/2;
		int Mid=asc[MidIndex];//找到中间元素
		System.out.print("中间元素index="+MidIndex+" 值为："+Mid+" value="+value);
		if(value==Mid)
		{
			System.out.println("找到了");
			return MidIndex;
		}
		//如果value比中间元素大,那么找右边
		else if(value>Mid)
		{
			System.out.println("进入右边 ");
			MidIndex=getPosition(asc, value, MidIndex+1, end);
		}else if(value<Mid)
		{
			System.out.println("进入左边 ");
			//如果value比中间元素小，那么找左边
			MidIndex=getPosition(asc, value, 0,MidIndex-1);
		}
		return MidIndex;
	
			
	}
	//找到数组array[lo...hi]中第i小的元素
	public int randomizedSelect(int array[],int lo,int hi,int i)
	{
		return 0;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	public static void main(String args[])
	{
		int asc[]={1,1,2,2,5,6,9,10,33,55,99,1231,3231};
		int position=getPosition(asc, 10000, 0, asc.length-1);
		System.out.println("目标元素下标为："+position);
	}

}
