/*
 * ���ܣ�д��һ���ļ�
 */
package com.IO;
import java.io.*;

public class Demo3_Write {

	public static void main(String[] args) {
		File f=new File("I:\\JavaText.txt");
		FileOutputStream fot=null;
		try {
			//����FileOutputStream��д������
			fot=new FileOutputStream(f);
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//����ļ����� ��д
		if(f.exists())
		{
			System.out.println("��ʼд��");
			//��ʼд���ȱ��浽byte���飬�ٴ�byte����д���ļ�
			String s=new String("My job is \n\r creat GameWorld!");
			//�����ֽ�����
			byte[] by=new byte[1024];
			//ѭ���ؽ��ַ������ݱ��浽�ֽ�����
				try {
					fot.write(s.getBytes());
				} catch (IOException e) {
					e.printStackTrace();
				}finally{
					try {
						System.out.println("д��ɹ������ڹر��ļ�");
						//�ر��ļ�
						fot.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}	
		else
		{
			System.out.println("д��ʧ�ܣ����ļ�������");
		}
	}

}
