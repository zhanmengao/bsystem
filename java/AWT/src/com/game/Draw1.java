/*
 * 功能：画一个圆、一个矩形、一条线段
 */
package com.game;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;

import javax.swing.*;

public class Draw1 extends JFrame{
	
	public Draw1()
	{
		MyPanel1 mp=new MyPanel1();
		//画好了，添加mp到JFrame
		this.add(mp);
		
		//设置容器属性
		this.setSize(1200, 750);
		this.setLocation(100, 100);
		this.setTitle("大家好我是Java");
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	}
	public static void main(String[] args) {
		Draw1 d1=new Draw1();

	}

}
class MyPanel1 extends JPanel{
	//重载绘图方法
	public void paint(Graphics g)//这个对象g就是我们的画笔
	{
		//记得调用一下父类构造方法
		super.paint(g);
		//下面开始绘图…………
//		g.drawOval(100, 100, 30, 30);
//		g.drawLine(0, 0, 50, 50);
//		g.drawRect(150, 150, 100, 80);
//		g.setColor(Color.blue);
//		g.fillRect(200, 200, 60, 20);
//		在画板上画出图片
		Image im=Toolkit.getDefaultToolkit().getImage
				(Panel.class.getResource("/Huli.jpg"));
		g.drawImage(im, 0,0, 1200, 750,this);
//		AffineTransform at=new AffineTransform();
//			
//			at.setToTranslation(50, 50);//平移
//			at.rotate(Math.PI/8);//旋转
//			at.setToScale(2, 2);//错切
//			at.scale(2, 2);//缩放
//			
//			Graphics2D g2=(Graphics2D) g;
//			g2.setTransform(at);
		
		//画出字
//		g.setColor(Color.LIGHT_GRAY);
//		g.setFont(new Font("微软雅黑",Font.BOLD,30));
//		g.drawString("我要学好Java和C#", 500, 500);

		
		
	}
	
}