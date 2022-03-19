/*
 * 4、编写一个程序，在控制台接收数据，程序会让用户输入名字，并检查用户的输入：
 * 不允许输入空名字：一旦用户完成输入，
 * 程序会向该用户发出问题：“Hello，你输入的名字”。
 * (提示：如果希望在控制台中与用户交互，一种可能的做法就是将System.in包装成)BufferedReader要做到这一点，
 * 必须使用InputStreamReader类将System.in
 * 这个InputStream对象转换成一个Reader即BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
 */
package com.pra;

import java.io.*;

public class Demo3_4 {
	public static void main(String []args)
	{
		Demo3_4 d=new Demo3_4();
		d.in();
	}
	public void in()
	{
		//写一个InputStream对象，接收控制台输入
		InputStreamReader isr=new InputStreamReader(System.in);
		//将isr转化成BufferedReader对象
		BufferedReader br=new BufferedReader(isr);
		
		
	   
	    try {
	    	while(true)
	    	{
	    	System.out.println("请输入您的名字");
			String name=br.readLine();	
			if(name.equals("0"))
			{
				System.exit(0);
			}
			System.out.println("Hello，"+name+"!");
	    	}
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
			try {
				isr.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
	    

		
	}

}
