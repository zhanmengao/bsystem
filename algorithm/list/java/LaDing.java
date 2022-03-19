package com.xianxingbiao;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class LaDing {
	static Cat head=null;
	static Cat array[]=null;//容器数组
	static int [][]lading=null;//结果数组
	//根据输入 构造一个循环链表
	public LaDing(int length)
	{
		lading=new int[length][length];
		array=new Cat[length];
		for(int i=0;i<length;i++)
		{
			Cat cat=new Cat(i+1);
			array[i]=cat;
			if(i==0){
				head=cat;
				continue;
			}
			array[i-1].setNextNode(cat);
			if(i==length-1)
			{
				cat.setNextNode(array[0]);
			}
			
		}
	}
	
	//构造拉丁方阵
	public static void main(String args[])
	{
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String shuru="";
		try {
			System.out.print("请输入拉丁方阵规模：");
			shuru=br.readLine();
		} catch (Exception e) {
			e.printStackTrace();
		}finally{
			 try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		LaDing MyLading=new LaDing(Integer.parseInt(shuru));
		Cat MyCatI=array[array.length-1];//外层循环的引用
		for(int i=0;i<array.length;i++)
		{
			MyCatI=(Cat) MyCatI.getNextNode();//外层循环的迭代
			Cat MyCatJ=MyCatI;//内层循环的引用
			for(int j=0;j<array.length;j++)
			{
				lading[i][j]=MyCatJ.point;
				MyCatJ=(Cat) MyCatJ.getNextNode();//内层循环的迭代
				System.out.print(lading[i][j]+"  ");
			}
			System.out.println();
		}
		
	}
}
class Cat extends Node1{
	public int point;
	public Cat(int point)
	{
		this.point=point;
	}
}
