package com.SortSeek;

public class CountSort {
	public CountSort(int array[],int min,int max)
	{
		//B����洢���������
		int B[]=new int[array.length];
		//c�����¼����[min,max]��ÿ�������ֵ�Ƶ��
		int C[]=new int[max-min];
		
		//��ʼ��C
		for(int i=0;i<C.length;i++)
		{
			C[i]=0;
		}
		//ͳ��ÿ��������Ƶ������¼��C����
		for(int j=0;j<array.length;j++)
		{
			C[array[j]]++;
		}		
		for(int j=0;j<C.length;j++)
		{
			//�Ե����϶�̬�滮��ͳ���ж�����С�ڵ�ǰ
			C[array[j]]+=C[array[j]-1];
		}
		//C�����ʱ�Ѿ��洢�� c[j]Ӧ�÷��ڵڼ���λ�ã�jΪ����ֵ��C[j]ΪС�ڵ����������ĸ���->j����ȷ�ڷ�λ��
		for(int j=B.length-1;j>=0;j--)
		{
			
			B[C[j]]=array[j];
			C[j]--;
		}
		array=B;
	}
}





