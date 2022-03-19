/*
 * 功能：写一个绘图框架
 */
package com.game;
import java.awt.*;
import javax.swing.*;

public class DrawFrame extends JFrame{
	//老规矩，构造方法里把我的画板的对象添加到JFrame
	public DrawFrame()
	{
		//实例化顶层容器和我的画板，把我的画板添加到顶层容器
	
		MyPanel mp=new MyPanel();
		this.add(mp);
		
		this.setSize(30, 30);
		this.setLocation(0, 0);
		
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		
	}
	public static void main(String[] args) {
		DrawFrame df=new DrawFrame();
	}

}
class MyPanel extends JPanel{
	//重载绘图方法
	public void paint(Graphics g)//这个对象g就是我们的画笔
	{
		//记得调用一下父类构造方法
		super.paint(g);
		//下面开始绘图…………
	}
	
}
