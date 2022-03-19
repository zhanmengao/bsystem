package com.StackAndQueue;

public class Fibonacci {
	//求斐波那契数列第n个数的值--自底向上的动态规划
	//输入:n
	//输出：Fibonacci[n-1]--从0开始计算，此时n-1即斐波那契数列第n个数的值
	public int FromFloor(int n)
	{
		int[] Fibonacci =new int[n];
		Fibonacci[0]=0;
		Fibonacci[1]=1;
		for(int i=2;i<n;i++)
		{
			Fibonacci[i]=Fibonacci[i-2]+Fibonacci[i-1];
		}
		return Fibonacci[n-1];
	}
	//求斐波那契数列第n个数的值--自顶向下的动态规划
	//输入:n
	//输出：Fibonacci[n-1]--从0开始计算，此时n-1即斐波那契数列第n个数的值
	public int FromTop(int n)
	{
		int[] Fibonacci =new int[n];
		Fibonacci[0]=0;
		Fibonacci[1]=1;
		//在这里截断递归：如果之前已经计算过了，就从该数组中取
		if(Fibonacci[n-1]!=0)
			return Fibonacci[n-1];
		//与一般递归式的不同：如果该数计算过，则递归式则会从数组中取出元素
		return Fibonacci[n-1]=FromTop(n-1)+FromTop(n-2);
	}
	
	public static void main(String args[])
	{

	}
	
	
	
	
	
}
