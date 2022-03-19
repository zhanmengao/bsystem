package com.lang.String;
/*
 * 实现字符串匹配算法————BF算法
 */

public class BF {
	public static int BF(String primary,String sub)
	{
		for(int i=0;i<primary.length()-sub.length()+1;i++)
		{
			//截取主串
			String temp=primary.substring(i, sub.length()+i);
			if(temp.equals(sub))
			{
				return i;
			}
		}
		return -1;
		
	}
	public static int BF2(String primary,String sub)
	{
		char char_pri[]=primary.toCharArray();
		char char_sub[]=sub.toCharArray();
		int i=0,j=0;
		while(i<char_pri.length && j<char_sub.length)
		{
			//如果当前元素相等，则后移
			if(char_pri[i]==char_sub[j])
			{
				i++;
				j++;
				if(j==sub.length())
				{
					return i-j;
				}
			}//如果不相等，则j（模式串）从头开始比
			else{
				i++;
				j=0;
			}
			
		}
		return -1;
	}
	public static void main(String args[])
	{
		int b=BF2("system","m");
		System.out.println(b);
	}
 
}
