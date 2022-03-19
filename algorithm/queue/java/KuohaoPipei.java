package com.StackAndQueue;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class KuohaoPipei {
	//判断括号是否匹配
	//输入：Java语句
	//输出：是否 error
	public boolean Pipei(String str)
	{
		boolean boo=true;
		LinkedStack linkedStack=new LinkedStack();
		char in[]=str.toCharArray();
		//循环取出
		for(char c:in)
		{
			//如果是左括号，入栈
			if('('==c || '{'==c || '['==c)
			{
				//入栈
				linkedStack.push(c);
			}
			else if(')'==c)
			{
				//如果栈里面匹配不到相应的左括号->报错
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//右括号前一个必是左括号，如果不是->报错
				if(a!='(')
				{
					boo=false;
					break;
				}	
			}
			else if('}'==c)
			{
				//如果栈里面匹配不到相应的左括号->报错
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//右括号前一个必是左括号，如果不是->报错
				if(a!='}')
				{
					boo=false;
					break;
				}	
			}else if('['==c)
			{
				//如果栈里面匹配不到相应的左括号->报错
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//右括号前一个必是左括号，如果不是->报错
				if(a!=']')
				{
					boo=false;
					break;
				}	
			}
		}
		//如果结束时，栈里面还有括号，则错误
		if(!linkedStack.IsEmpty())
			boo=false;
		
		return boo;
	}
	public static void main(String args[])
	{
		KuohaoPipei kuohaoPipei=new KuohaoPipei();
		System.out.println(kuohaoPipei.Pipei("aaa"));
	}
}
