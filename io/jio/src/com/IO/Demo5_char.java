/*
 * ���ܣ��ַ��� FileReader,FileWriter
 */
package com.IO;
import java.io.*;

public class Demo5_char {

	public static void main(String[] args) {
		FileReader fr=null;
		FileWriter fw=null;
		
		try {
			fr=new FileReader("F:\\Java�ؼ����б�.txt");
			fw=new FileWriter("C:\\Users\\��\\Documents\\22.txt");
			
			//ѭ����ȡ
			char c[]=new char[1024]; 
			int n=0;
			//n=-1��ʾ������
			//��ʾ��cȥ����fr�����ݣ�n��ʾ�������ֽ�����ĵڼ�λ
			while((n=fr.read(c))!=-1)
			{
				//û�������Ͱ���д��fw
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
