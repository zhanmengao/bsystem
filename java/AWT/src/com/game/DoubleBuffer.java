/*
 * 功能：使用双缓冲机制绘图（离屏绘制、前屏显示）
 * 作用：防止画面闪烁
 */
package com.game;

import javax.swing.*;
import java.awt.*;

public class DoubleBuffer extends JFrame{
	MyPanela mpa=null;
	public DoubleBuffer()
	{
		mpa=new MyPanela();
		this.add(mpa);

		
		this.setSize(400,400);
		this.setLocation(500, 500);
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) {
		DoubleBuffer db=new DoubleBuffer();
	}
}
class MyPanela extends JPanel{
	Image im=null;
	//绘图方法
	private int x=0;
	private int y=0;
	private int diameer=5;

	
	public void paint(Graphics g)
	{
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);
		while(x<300)
		{
			this.gameLoop();
			this.gameRender();
			this.gamepaint();
		}

	}
	//更新游戏逻辑
	public void gameLoop()
	{
		x+=5;
	}
	//离屏绘制
	public void gameRender()
	{
		System.out.println("进入离屏绘制");
		//建立窗口大小的Image对象
		Image im=createImage(400,400);
		//获取im的Graphics对象
		Graphics dbg=im.getGraphics();
		//设置画笔颜色
		dbg.setColor(Color.white);
		//画一个小球
		dbg.fillOval(x, 200, diameer, diameer);
		System.out.println("绘画完毕！");
	}
	//前屏显示
	public void gamepaint()
	{
		System.out.println("进入前屏显示");
		Graphics g=this.getGraphics();
		//将图像缓冲区的内容复制到前屏缓冲区
		g.drawImage(im,0,0,null);
		//释放当前屏幕的Graphics对象
		g.dispose();
		System.out.println("复制离屏画到前屏！");
	}

	
	
	
	
	
	
	
}
