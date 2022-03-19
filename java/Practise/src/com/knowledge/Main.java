package com.knowledge;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    //输入带空格分割的字符串
    public static void main(String args[])
    {
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));  
        String str = null;
		try {
			str = br.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
	        try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
        //遍历
        char []arr=str.toCharArray();
        int j=arr.length-1;
        for(int i=arr.length-1;i>=0;i--)
        {
        	//如果遇到第一个空格
        	if(arr[i]==' '||i==0)
        	{
        		if(i>=arr.length-1)
        		{
        			continue;
        		}
        		//从i开始，输出字符数组i+1到j所有内容
        		for(int k=i+1;k<=j;k++)
        		{
        			if(k-1==0)
        			{
        				System.out.print(arr[i]);
        			}
        			if(arr[k]!=' ')
        			{
        				System.out.print(arr[k]);
        				if(k==j && k!=0)
        				{
        					System.out.print(" ");
        				}
        			}
        		}
        		if(i==0)
        		{
        			break;
        		}
        		//找到下一个非空格
        		while(arr[i]==' ')
        		{
        			i--;
        			if(i<=0)
        			{
        				break;
        			}
        		}
        		j=i;
        		
        	}
        }
        

    }

}
