/*
 * 功能：练习监听：上下左右小球走
 */
package com.game;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;

public class Ball1 extends JFrame {
	MyPanel4 mp4=null;
	Ball1()
	{
		mp4=new MyPanel4();
		this.addKeyListener(mp4);
		
		this.add(mp4);
		this.setSize(300,300);
		this.setLocation(500,500);
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	

	public static void main(String[] args) {
		System.out.println("主函数");
		Ball1 ball1=new Ball1();

	}

}
class MyPanel4 extends JPanel implements KeyListener,Runnable{
	int x=10;int y=10;
	//游戏循环：小球不断向右走

	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.blue);
		g.fillOval(x, y, 10, 10);
	}
	

	
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void keyPressed(KeyEvent e) {
		int keycode=e.getKeyCode();
		switch(keycode)
		{
		case KeyEvent.VK_DOWN:
			System.out.println("下！");
			y+=3;
			break;
		case KeyEvent.VK_UP:
			System.out.println("上");
			y-=3;
			break;
		case KeyEvent.VK_LEFT:
			System.out.println("左");
			x-=3;
			break;
		case KeyEvent.VK_RIGHT:
			System.out.println("右");
			x+=3;
			break;
		}
		this.repaint();
		
	}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	//控制我的画板更新页面
		public void run() 
		{
			while(true)
			{
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				this.repaint();
			}

		}
	}
