/*
 * ���ܣ���ȡһ���ļ�����Ϣ������һ���ļ����ļ���(����Ѿ���ͬ�����ٴ����ᷢ�����ǣ�)
 */
package com.IO;
import java.io.*;

public class Demo1 {
	

	public static void main(String[] args) {
		//����һ���ļ�����
		File f=new File("C:\\Users\\Administrator\\Desktop");	
		System.out.println("�ҽ�"+f.getName());
		System.out.println("��ס��"+f.getAbsolutePath());
		System.out.println("δ����"+f.getFreeSpace()+"�ֽ�");
		System.out.println("��Ŀ¼"+f.getParentFile());
		System.out.println("�ɶ���д��"+f.canRead());
		
		//����һ���ļ�
		File f1=new File("I:/JavaText.txt");
		//����ļ�������
		if(f1.exists()==false)
		{
			System.out.println("���Դ�����");
			try {
				//����һ���ļ�
				f1.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}//δ�漰�ļ�I/O������Ҫ�ر�
		}
		else
			System.out.println("�ļ��Ѵ��ڣ����ܴ���");
		
		
		//����һ���ļ���
		File f2=new File("I:/JavaT()");
		//�ж϶����ַ�Ƿ���һ���ļ���
		if(f2.isDirectory())
		{
			//�����һ���ļ��У�˵�����ļ����Ѵ���
			System.out.println("�ļ����Ѵ���,����ʧ��");
		}
		else
		{
			//����
			f2.mkdir();
		} 
		
		//��ȡһ���ļ����������ļ�����File�����������
		File f3=new File("I:\\�����\\xuniji");
		
		if(f3.isDirectory())
		{
			//��ȡ�ļ����������ļ�,�ŵ��ļ���������
			File[] ff=f3.listFiles();
			for(int i=0;i<ff.length;i++)
			{
				System.out.println("��"+(i+1)+"���ļ���Ϊ"+ff[i].getName());
			}
			
		}
	}
	

}
