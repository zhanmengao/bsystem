/*
 * 3、编写两个方法分别计算合数n!.一个使用递归方法实现，另一个不用递归方法。
 */
package com.pra;

import java.util.Scanner;

public class Demo3_3 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
	}
	//递归实现计算合数n! 输入：顶层元素n 输出：n!
	public int recursion(int n)
	{
		//如果当前是基本情形，返回1
		if(n==0||n==1)
		{
			return 1;
		}
		//如果当前还需要继续算，则迭代（会造成n*(n-1)*(n-2)*……)，直到基本情形
		else 
		{
			return n*recursion(n-1);
		}
		
	}
	//循环计算累乘
	public int loop()
	{
		Scanner sr=new Scanner(System.in);
		//从控制台读取n
		int n=sr.nextInt();
		int accumulate=1;
		//只要n大于0就继续计算
		while(n>0)
		{
			accumulate=accumulate*n;
			n--;
		}
		return accumulate;
	}
}
