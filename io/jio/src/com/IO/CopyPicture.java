/*
 * ���ܣ�����ͼƬ/�ļ�
 */
package com.IO;
import java.io.*;

public class CopyPicture {
	public static void main(String []args)
	{
		FileInputStream fis=null;
		FileOutputStream fos=null;
		
		try {
			fis=new FileInputStream("H:\\Myeclipse\\workspace\\Game\\src\\11.jpg");
			fos=new FileOutputStream("C:\\Users\\��\\Pictures\\Camera Roll\\22.jpg");
			
			//ѭ����ȡ�ֽ���
			byte by[]=new byte[1024];
			int n=0;
			while((n=fis.read(by))!=-1)
			{
				fos.write(by, 0, n);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
			try {
				fis.close();
				fos.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
	}

}
