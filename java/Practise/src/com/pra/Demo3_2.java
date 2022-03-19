/*
 * 2、请编写一个程序，实现从控制台读入字符，接收到"e"或者"E"退出程序
 */
package com.pra;

import java.io.*;

public class Demo3_2 {

	public static void main(String[] args) {
		Demo3_2 d=new Demo3_2();
		d.readchar();
		
	}
	//接收字符的函数
	public void readchar()
	{

			//定义一个字符流is,该字符流从控制台读取字符
		    InputStreamReader is=null;
			BufferedReader br=null;
			try {
				while(true)
				{
				is=new InputStreamReader(System.in) ;
				br=new BufferedReader(is) ;
				System.out.println("请输入字符");
				String s=br.readLine();
				if(s.equals("E")||s.equals("e"))
				{
					System.out.println("窗口关闭！");
					System.exit(0);
				}
			} 
			}
				catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}finally
			{
				try {
					br.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				try {
					is.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
			
		}

	

}
