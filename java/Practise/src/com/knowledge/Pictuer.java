/*˼·��ʹ�� java.awt.Image���µ�Image���Խ���ͼƬ����ȡ��ʹ��ImageIO����
	�������£�
	
 * ��ȡͼƬ�����ȵ������µİ�
 */
package com.knowledge;
import java.awt.Image;
import javax.imageio.ImageIO;
import java.io.*;

public class Pictuer {
	/**
	         * ��Image����������ͼƬ
	         * ·������ʵ������޸�
	         */
	public static void main(String []args)
	{
        Image image;
		try 
		{
			image = ImageIO.read(new File("p:\\IMG_1410.jpg"));
			System.out.println(image.getSource());
			System.out.println("�и�ͼƬ!");
		} 
		catch (IOException e)
		{
			e.printStackTrace();
			System.out.println("???");
		}     
	}
}
