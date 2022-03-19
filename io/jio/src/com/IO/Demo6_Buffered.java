/*
 * �����ַ���BufferedReader,BufferedWriter��
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
			//����FileReader��FileWrter����
			fr=new FileReader("F:\\Java�ؼ����б�.txt");
			fw=new FileWriter("C:\\Users\\��\\Documents\\BufferedWriter.txt");
			//��fr��fw����������ת����BufferedReader��BufferedWriter����
			br=new BufferedReader(fr);
			bw=new BufferedWriter(fw);
			
			//ѭ����ȡ��ÿ�ζ�һ��
			String s="";
			int i=1;
			//���ַ����������ݣ���Ϊ��ʱ������
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
