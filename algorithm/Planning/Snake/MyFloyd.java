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
	//输入：一个原始距离数组 和一个经过计算距离的数组
	public static void Floyd(int c[][],int a[][])
	{
		int i,j,k;
		int n=c[0].length;
		//先把题给矩阵c 作为我们的初始矩阵a
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				a[i][j]=c[i][j];
			}
		}
		//将对角线（到自己的路线的距离）：清零
		for(i=0;i<n;i++)
		{
			a[i][i]=0;
		}
		//开始迭代：一次即搜索所有对于i和j的值，检查顶点是否比直达更优秀（或者是否可以中转到达）
		for(k=0;k<n;k++)
		{
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					//如果直达距离大于中转距离，就将直达距离作为我们的最短路径
					if(a[i][j]>a[i][k]+a[k][j])
					{
						a[i][j]=a[i][k]+a[k][j];
					}
				}
			}
		}
	}

}

