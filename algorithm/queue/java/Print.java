package com.StackAndQueue;
/*
 * 功能：逆置一切
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;

public class Print {
	public static void main(String args[]) throws Exception
	{
		System.out.print("请输入序列：");
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String in=br.readLine();
		LinkedStack stack=new LinkedStack();
		
		//将输出String转化为char[]
		char charin[]=in.toCharArray();	
		//逆置
		for(int i=0,j=charin.length-1;i<charin.length/2;i++,j--)
		{
			char temp=charin[i];
			charin[i]=charin[j];
			charin[j]=temp;
		}
		/*
		 * //组织char[]入栈
		
		for(int i=0;i<charin.length;i++)
		{
			stack.push(charin[i]);
		}
		//出栈
		while(!stack.IsEmpty())
		{
			System.out.print(stack.pop());
		}
		 */
		
	}

}




