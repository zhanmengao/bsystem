/*
 * 功能：读一个文件
 */
package com.IO;
import java.io.*;

public class Demo2_Read {

	public static void main(String[] args){
		File f=new File("I:\\新建文本文档 (2).txt");
		
		FileInputStream fis = null;
		try {
			//借用FileInputStream的读文件能力
			fis = new FileInputStream(f);
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		
		//判断是否存在该文件
		if(f.exists()==true)
		{
			//可以读，用一个字节数组接收
			byte []by=new byte[1024];
			int n=0;
			
			try {
				//把fis里面的内容读到数组by中,返回-1表示读完了，那么不等于-1就继续读
				while((n=fis.read(by))!=-1)
				{
					//将byte数组内容转化为字符串
					String s=new String(by,0,n);
					//输出读到的内容
					System.out.println(s);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}finally{
				try {
					//关闭文件
					fis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}	
		}
		else
		{
			System.out.println("找不到指定路径");
		}
	}

}
