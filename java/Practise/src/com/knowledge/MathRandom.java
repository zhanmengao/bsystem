/*
 * 功能：提供一个写随机数的方法
 */
package com.knowledge;

public class MathRandom {
	public static  int GetRandom(double start,double end)
	{
		//产生start~end之间的随机数
		int s=(int)start+(int)Math.random()*(int)(end-start);
		return s;
	}
	public static void main(String []args)
	{
		for(int i=0;i<5;i++)
		{
			System.out.println(GetRandom(1,4));
			System.out.println((int)Math.random());
		}
		char c='a';
		switch(c){
		case 'a':
			System.out.println("a");
		default:
			System.out.println("default");
		}
		
		
		
	}

}
