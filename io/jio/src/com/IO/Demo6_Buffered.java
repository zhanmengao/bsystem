/*
 * 缓冲字符流BufferedReader,BufferedWriter，
 */
package com.IO;

import java.io.*;

public class Demo6_Buffered {

	public static void main(String[] args) {
		FileReader fr=null;
		BufferedReader br=null;
		FileWriter fw=null;
		BufferedWriter bw=null;
		
		
		try {
			//创建FileReader和FileWrter对象
			fr=new FileReader("F:\\Java关键字列表.txt");
			fw=new FileWriter("C:\\Users\\必\\Documents\\BufferedWriter.txt");
			//把fr和fw这两个对象转化成BufferedReader和BufferedWriter对象
			br=new BufferedReader(fr);
			bw=new BufferedWriter(fw);
			
			//循环读取：每次读一行
			String s="";
			int i=1;
			//用字符串记下内容，不为空时继续读
			while((s=br.readLine())!=null)
			{
				System.out.println(s+(i++));	
				bw.write(s);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				fr.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}try {
			fw.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		

	}

}
