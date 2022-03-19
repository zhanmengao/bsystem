package com.xianxingbiao;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class LaDing {
	static Cat head=null;
	static Cat array[]=null;//��������
	static int [][]lading=null;//�������
	//�������� ����һ��ѭ������
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
	
	//������������
	public static void main(String args[])
	{
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String shuru="";
		try {
			System.out.print("���������������ģ��");
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
		Cat MyCatI=array[array.length-1];//���ѭ��������
		for(int i=0;i<array.length;i++)
		{
			MyCatI=(Cat) MyCatI.getNextNode();//���ѭ���ĵ���
			Cat MyCatJ=MyCatI;//�ڲ�ѭ��������
			for(int j=0;j<array.length;j++)
			{
				lading[i][j]=MyCatJ.point;
				MyCatJ=(Cat) MyCatJ.getNextNode();//�ڲ�ѭ���ĵ���
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
