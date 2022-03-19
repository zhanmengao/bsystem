package com.lang.String;
/*
 * 模式串：主串
 * 目标：子串
 */

public class KMP {
	public static int index_KMP(String primary,String sub,int begin)
	{
		int next[]=getNext(sub);
		//对BF算法进行改进
		char char_pri[]=primary.substring(begin).toCharArray();//主串
		char char_sub[]=sub.toCharArray();//模式串
		int i=0,j=0;
		while(i<char_pri.length && j<char_sub.length)
		{
			//如果当前元素相等，则后移
			if(char_pri[i]==char_sub[j])
			{
				i++;
				j++;
				//如果此时匹配到尾部
				if(j==sub.length())
				{
					return i-j;
				}
			}
			else if(j>0){
				//如果主串与子串发生不相等，则从next[j-1]开始重新匹配
				j=next[j-1];//next[j-1]：表示j前面有next[j-1]个数与它相等->那么我们就从第一个与i相等的j开始匹配
			}else{
				//如果第一个就不相等，则主串指针i直接后移
				i++;
			}
			
		}
		return -1;
		
	}
	//功能：构造KMP的next[]数组
	private static int [] getNext(String sub)
	{
		char in[]=sub.toCharArray();
		int []next=new int[sub.length()];//做一个与原字符串长度相等的next数组->指示字符串下一次的匹配
		next[0]=0;
		int top=1;//后缀下标
		int bottom=0;//前缀下标
		while(top<next.length)
		{
			if(in[top]==in[bottom])
			{
				next[top]=bottom+1;//next[j]=模式串前后缀相等的元素个数n+自己也相等
				//后移
				top++;
				bottom++;		
			}else if(bottom>0){
				//回朔：当前字符的前缀与后缀不连续，如果在此处发生P与T的不匹配，则找到next[j-1]的位置开始下一次匹配
				bottom=next[bottom-1];
				
				//本次循环i不变，j会回朔到之前的索引位置，与当前in[i]发生比较
			}else{
				//如果此时在跟char[0]比较且不相等->next[i]=0
				next[bottom]=0;
				top++;
			}
		}
		return next;
	}
	public static void main(String args[])
	{
		System.out.println(index_KMP("美国短毛猫，又称美洲短毛虎纹猫，是美国人把欧洲猫与美洲土种猫加以改良而育成，是家猫中的传统品种。1971年，其被选为美国最好的猫种之一","欧洲猫与美洲土种猫",0));
	}
}
