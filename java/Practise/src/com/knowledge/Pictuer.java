/*思路：使用 java.awt.Image包下的Image可以接收图片。读取则使用ImageIO对象。
	代码如下：
	
 * 读取图片，首先导入以下的包
 */
package com.knowledge;
import java.awt.Image;
import javax.imageio.ImageIO;
import java.io.*;

public class Pictuer {
	/**
	         * 用Image对象来接收图片
	         * 路径根据实际情况修改
	         */
	public static void main(String []args)
	{
        Image image;
		try 
		{
			image = ImageIO.read(new File("p:\\IMG_1410.jpg"));
			System.out.println(image.getSource());
			System.out.println("有该图片!");
		} 
		catch (IOException e)
		{
			e.printStackTrace();
			System.out.println("???");
		}     
	}
}
