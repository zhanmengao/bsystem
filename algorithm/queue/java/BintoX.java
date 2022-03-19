package com.StackAndQueue;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class BintoX {
	ArrayStack Stake=null;
	static BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
	//功能：将二进制的char序列转化为十进制
	public void toDec(int[] array)
	{
		//将array中的元素入栈
		Stake=new ArrayStack(array.length);
		for(int i=0;i<array.length;i++)
		{
			Stake.Push(array[i]);
		}
		//取出数据
		int dec=0;
		for(int i=0;i<array.length;i++)
		{
			int temp=(int) Stake.pop();
			dec+=temp*Math.pow(2,i);
		}
		System.out.println(dec);
	}
	public static void main(String args[]) throws Exception
	{
		
		BintoX bintox=new BintoX();
		System.out.print("请输入二进制序列：");
		String StringNum=br.readLine();
		String[] arrayStringNum=StringNum.split("");
		int []num=new int[arrayStringNum.length];
		for(int i=0;i<arrayStringNum.length;i++)
		{
			num[i]=Integer.parseInt(arrayStringNum[i]);
		}
		bintox.toDec(num);
	}

}
