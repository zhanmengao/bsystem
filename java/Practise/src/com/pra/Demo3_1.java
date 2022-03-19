/*
 * 1、用循环控制语句打印输出：1+3+5+...+99=?结果
 */
package com.pra;

public class Demo3_1 {

	public static void main(String[] args) {
		Demo3_1 d=new Demo3_1();
		// TODO Auto-generated method stub
		System.out.println(d.sum());
	}
	public int sum()
	{
		int sum=0;
		for(int i=1;i<=99;i+=2)
		{
			sum+=i;
		}
		return sum;
		
	}

}
