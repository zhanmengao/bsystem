package com.StackAndQueue;
/*
 * ���ܣ�����һ��
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Stack;

public class Print {
	public static void main(String args[]) throws Exception
	{
		System.out.print("���������У�");
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String in=br.readLine();
		LinkedStack stack=new LinkedStack();
		
		//�����Stringת��Ϊchar[]
		char charin[]=in.toCharArray();	
		//����
		for(int i=0,j=charin.length-1;i<charin.length/2;i++,j--)
		{
			char temp=charin[i];
			charin[i]=charin[j];
			charin[j]=temp;
		}
		/*
		 * //��֯char[]��ջ
		
		for(int i=0;i<charin.length;i++)
		{
			stack.push(charin[i]);
		}
		//��ջ
		while(!stack.IsEmpty())
		{
			System.out.print(stack.pop());
		}
		 */
		
	}

}




