package com.SortSeek;
/*
 * 实现归并排序
 */

public class MergeSort {
	//归并排序
	//输入：需要排序的数组，左边索引位置，右边索引位置
	public static void sort(double array[],int lo,int hi)
	{
		//基本问题
		if(lo>=hi)
			return;
		//递归
		sort(array,lo,lo+(hi-lo)/2);
		sort(array,lo+(hi-lo)/2+1,hi);
		Mere(array,lo,hi);
	}
	
	//归并 输入：数组，起始,终止
	private static void  Mere(double []array,int lo,int hi)
	{
		//思想：将左右的有序数组依次放入原数组中
		//辅助数组
		double temparr[]=new double[hi-lo+1];
		//复制一份数组，方便取出每个数据
		for(int k=0;k<temparr.length;k++)
		{
			temparr[k]=array[k+lo];
		}
		int mid=(hi-lo)/2;
		//左下标
		int i=0;
		//右下标
		int j=mid+1;
		//遍历数组，将元素放到array数组的正确位置
		for(int k=lo;k<=hi;k++)
		{
			if(i>mid)//如果左边索引临界
			{
				array[k]=temparr[j++];
			}
			else if(j>=temparr.length)//如果右边临界
			{
				array[k]=temparr[i++];
			}
			//如果left下标的数组小
			else if(temparr[i]<=temparr[j])
			{
				array[k]=temparr[i];
				i++;
			}
			//如果right下标的数组小
			else{
				array[k]=temparr[j];
				j++;
			}	
		}
	}
	public static void print(double []array)
	{
		for(int i=0;i<array.length;i++)
		{
			System.out.print(array[i]+" ");
		}
		System.out.println();
	}
	public static void main(String args[])
	{
		double array[] = { 1, 9, 3, 6, 4, 5 };
		print(array);
		sort(array,0, array.length-1);
		print(array);
	}
}








