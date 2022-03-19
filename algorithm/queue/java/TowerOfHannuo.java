package com.StackAndQueue;
/*
 * 实现汉诺塔问题：根据现实中解决此问题的步骤，我们拟定算法
 * 
 */

public class TowerOfHannuo {
	public static void main(String args[])
	{
		move(3, 'a', 'c', 'b');
	}
	public static void move(int n,char from,char to,char sup)
	{
		//退出递归条件：n=1，即最上面那根，直接移
		if(n==1)
		{
			System.out.println(from+"->"+to);
		}else{
			move(n-1,from,sup,to);//1、将from的n-1个先移到sup
			System.out.println(from+"->"+to);//2、将from的第n个球移到to
			move(n-1,sup,to,from);//3、将sup的第n-1个球移到to
		}
	}

}
