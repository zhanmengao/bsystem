package com.SortSeek;

public class Select {
	//����һ��������ָ��Ԫ�ص�λ��
	//���룺�������飬ָ��Ԫ�أ���ʼλ�ã�����λ�� --��0Ϊ��ʼ
	//�����position
	public static int getPosition(int []asc,int value,int begin,int end)
	{
		if(begin>end || end>asc.length)//�������
		{
			System.out.println("û���ҵ�");
			return Integer.MAX_VALUE;
		}
		int MidIndex=begin+(end-begin)/2;
		int Mid=asc[MidIndex];//�ҵ��м�Ԫ��
		System.out.print("�м�Ԫ��index="+MidIndex+" ֵΪ��"+Mid+" value="+value);
		if(value==Mid)
		{
			System.out.println("�ҵ���");
			return MidIndex;
		}
		//���value���м�Ԫ�ش�,��ô���ұ�
		else if(value>Mid)
		{
			System.out.println("�����ұ� ");
			MidIndex=getPosition(asc, value, MidIndex+1, end);
		}else if(value<Mid)
		{
			System.out.println("������� ");
			//���value���м�Ԫ��С����ô�����
			MidIndex=getPosition(asc, value, 0,MidIndex-1);
		}
		return MidIndex;
	
			
	}
	//�ҵ�����array[lo...hi]�е�iС��Ԫ��
	public int randomizedSelect(int array[],int lo,int hi,int i)
	{
		return 0;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	public static void main(String args[])
	{
		int asc[]={1,1,2,2,5,6,9,10,33,55,99,1231,3231};
		int position=getPosition(asc, 10000, 0, asc.length-1);
		System.out.println("Ŀ��Ԫ���±�Ϊ��"+position);
	}

}
