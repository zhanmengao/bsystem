package com.Snake;


public class MyFloyd {

	public static void main(String []args)
	{
		int [][]c={{0,3,1000,2},
				{2,0,1000,1000},
				{10,7,0,1},
				{6,1000,1000,0}};
		int [][]a=new int[4][4];
		Floyd(c, a);
		for(int i=0;i<a[0].length;i++)
		{
			for(int j=0;j<a[0].length;j++)
			{
				System.out.print(a[i][j]+"  ");
			}
			System.out.println();
		}
	}
	//���룺һ��ԭʼ�������� ��һ������������������
	public static void Floyd(int c[][],int a[][])
	{
		int i,j,k;
		int n=c[0].length;
		//�Ȱ��������c ��Ϊ���ǵĳ�ʼ����a
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				a[i][j]=c[i][j];
			}
		}
		//���Խ��ߣ����Լ���·�ߵľ��룩������
		for(i=0;i<n;i++)
		{
			a[i][i]=0;
		}
		//��ʼ������һ�μ��������ж���i��j��ֵ����鶥���Ƿ��ֱ������㣨�����Ƿ������ת���
		for(k=0;k<n;k++)
		{
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					//���ֱ����������ת���룬�ͽ�ֱ�������Ϊ���ǵ����·��
					if(a[i][j]>a[i][k]+a[k][j])
					{
						a[i][j]=a[i][k]+a[k][j];
					}
				}
			}
		}
	}

}

