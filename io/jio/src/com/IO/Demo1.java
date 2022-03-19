/*
 * 功能：获取一个文件的信息，创建一个文件或文件夹(如果已经有同名，再创建会发生覆盖！)
 */
package com.IO;
import java.io.*;

public class Demo1 {
	

	public static void main(String[] args) {
		//构造一个文件对象
		File f=new File("C:\\Users\\Administrator\\Desktop");	
		System.out.println("我叫"+f.getName());
		System.out.println("我住在"+f.getAbsolutePath());
		System.out.println("未分配"+f.getFreeSpace()+"字节");
		System.out.println("父目录"+f.getParentFile());
		System.out.println("可读可写吗："+f.canRead());
		
		//创建一个文件
		File f1=new File("I:/JavaText.txt");
		//如果文件不存在
		if(f1.exists()==false)
		{
			System.out.println("可以创建！");
			try {
				//创建一个文件
				f1.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}//未涉及文件I/O，不需要关闭
		}
		else
			System.out.println("文件已存在，不能创建");
		
		
		//创建一个文件夹
		File f2=new File("I:/JavaT()");
		//判断对象地址是否是一个文件夹
		if(f2.isDirectory())
		{
			//如果是一个文件夹，说明该文件夹已存在
			System.out.println("文件夹已存在,创建失败");
		}
		else
		{
			//创建
			f2.mkdir();
		} 
		
		//获取一个文件夹下所有文件（用File对象数组接收
		File f3=new File("I:\\虚拟机\\xuniji");
		
		if(f3.isDirectory())
		{
			//获取文件夹下所有文件,放到文件对象数组
			File[] ff=f3.listFiles();
			for(int i=0;i<ff.length;i++)
			{
				System.out.println("第"+(i+1)+"个文件名为"+ff[i].getName());
			}
			
		}
	}
	

}
