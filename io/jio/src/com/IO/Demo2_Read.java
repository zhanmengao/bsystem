/*
 * ���ܣ���һ���ļ�
 */
package com.IO;
import java.io.*;

public class Demo2_Read {

	public static void main(String[] args){
		File f=new File("I:\\�½��ı��ĵ� (2).txt");
		
		FileInputStream fis = null;
		try {
			//����FileInputStream�Ķ��ļ�����
			fis = new FileInputStream(f);
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		
		//�ж��Ƿ���ڸ��ļ�
		if(f.exists()==true)
		{
			//���Զ�����һ���ֽ��������
			byte []by=new byte[1024];
			int n=0;
			
			try {
				//��fis��������ݶ�������by��,����-1��ʾ�����ˣ���ô������-1�ͼ�����
				while((n=fis.read(by))!=-1)
				{
					//��byte��������ת��Ϊ�ַ���
					String s=new String(by,0,n);
					//�������������
					System.out.println(s);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}finally{
				try {
					//�ر��ļ�
					fis.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}	
		}
		else
		{
			System.out.println("�Ҳ���ָ��·��");
		}
	}

}
