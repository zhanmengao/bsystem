package com.SortSeek;
/*
 * ʵ�ֹ鲢����
 */

public class MergeSort {
	//�鲢����
	//���룺��Ҫ��������飬�������λ�ã��ұ�����λ��
	public static void sort(double array[],int lo,int hi)
	{
		//��������
		if(lo>=hi)
			return;
		//�ݹ�
		sort(array,lo,lo+(hi-lo)/2);
		sort(array,lo+(hi-lo)/2+1,hi);
		Mere(array,lo,hi);
	}
	
	//�鲢 ���룺���飬��ʼ,��ֹ
	private static void  Mere(double []array,int lo,int hi)
	{
		//˼�룺�����ҵ������������η���ԭ������
		//��������
		double temparr[]=new double[hi-lo+1];
		//����һ�����飬����ȡ��ÿ������
		for(int k=0;k<temparr.length;k++)
		{
			temparr[k]=array[k+lo];
		}
		int mid=(hi-lo)/2;
		//���±�
		int i=0;
		//���±�
		int j=mid+1;
		//�������飬��Ԫ�طŵ�array�������ȷλ��
		for(int k=lo;k<=hi;k++)
		{
			if(i>mid)//�����������ٽ�
			{
				array[k]=temparr[j++];
			}
			else if(j>=temparr.length)//����ұ��ٽ�
			{
				array[k]=temparr[i++];
			}
			//���left�±������С
			else if(temparr[i]<=temparr[j])
			{
				array[k]=temparr[i];
				i++;
			}
			//���right�±������С
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








