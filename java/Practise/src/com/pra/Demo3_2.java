/*
 * 2�����дһ������ʵ�ִӿ���̨�����ַ������յ�"e"����"E"�˳�����
 */
package com.pra;

import java.io.*;

public class Demo3_2 {

	public static void main(String[] args) {
		Demo3_2 d=new Demo3_2();
		d.readchar();
		
	}
	//�����ַ��ĺ���
	public void readchar()
	{

			//����һ���ַ���is,���ַ����ӿ���̨��ȡ�ַ�
		    InputStreamReader is=null;
			BufferedReader br=null;
			try {
				while(true)
				{
				is=new InputStreamReader(System.in) ;
				br=new BufferedReader(is) ;
				System.out.println("�������ַ�");
				String s=br.readLine();
				if(s.equals("E")||s.equals("e"))
				{
					System.out.println("���ڹرգ�");
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
