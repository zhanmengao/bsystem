/*
 * 4����дһ�������ڿ���̨�������ݣ���������û��������֣�������û������룺
 * ��������������֣�һ���û�������룬
 * ���������û��������⣺��Hello������������֡���
 * (��ʾ�����ϣ���ڿ���̨�����û�������һ�ֿ��ܵ��������ǽ�System.in��װ��)BufferedReaderҪ������һ�㣬
 * ����ʹ��InputStreamReader�ཫSystem.in
 * ���InputStream����ת����һ��Reader��BufferedReader in=new BufferedReader(new InputStreamReader(System.in));
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
		//дһ��InputStream���󣬽��տ���̨����
		InputStreamReader isr=new InputStreamReader(System.in);
		//��isrת����BufferedReader����
		BufferedReader br=new BufferedReader(isr);
		
		
	   
	    try {
	    	while(true)
	    	{
	    	System.out.println("��������������");
			String name=br.readLine();	
			if(name.equals("0"))
			{
				System.exit(0);
			}
			System.out.println("Hello��"+name+"!");
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
