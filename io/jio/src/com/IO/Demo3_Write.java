/*
 * 功能：写入一个文件
 */
package com.IO;
import java.io.*;

public class Demo3_Write {

	public static void main(String[] args) {
		File f=new File("I:\\JavaText.txt");
		FileOutputStream fot=null;
		try {
			//借用FileOutputStream的写入能力
			fot=new FileOutputStream(f);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//如果文件存在 则写
		if(f.exists())
		{
			System.out.println("开始写入");
			//开始写，先保存到byte数组，再从byte数组写到文件
			String s=new String("My job is \n\r creat GameWorld!");
			//定义字节数组
			byte[] by=new byte[1024];
			//循环地将字符串内容保存到字节数组
				try {
					fot.write(s.getBytes());
				} catch (IOException e) {
					e.printStackTrace();
				}finally{
					try {
						System.out.println("写入成功！正在关闭文件");
						//关闭文件
						fot.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}	
		else
		{
			System.out.println("写入失败！该文件不存在");
		}
	}

}
