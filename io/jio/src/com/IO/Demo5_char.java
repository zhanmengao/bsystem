/*
 * 功能：字符流 FileReader,FileWriter
 */
package com.IO;
import java.io.*;

public class Demo5_char {

	public static void main(String[] args) {
		FileReader fr=null;
		FileWriter fw=null;
		
		try {
			fr=new FileReader("F:\\Java关键字列表.txt");
			fw=new FileWriter("C:\\Users\\必\\Documents\\22.txt");
			
			//循环读取
			char c[]=new char[1024]; 
			int n=0;
			//n=-1表示读完了
			//表示用c去缓存fr的内容，n表示读到了字节数组的第几位
			while((n=fr.read(c))!=-1)
			{
				//没结束，就把它写到fw
				fw.write(c, 0, n);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}finally{
			try {
				fr.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}try {
				fw.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}

	}

}
