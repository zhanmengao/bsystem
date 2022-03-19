/*
 * 功能：构建游戏循环
 * 用法：空循环
 * 作用：使线程能接收到每一个命令
 * 
 */
package com.game;
import javax.swing.*;
import java.awt.*;

public class Loop extends JFrame {
	MotionOneself mo=null;
	RestBall rb=null;
	
	public Loop()
	{
		mo=new MotionOneself();
		rb=new RestBall();
		this.add(mo);
		
		this.setSize(400,400);
		this.setLocation(500, 500);
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) {
		Loop lp=new Loop();
	}

}
//小球是静止的————变量值不改变
class RestBall extends JPanel{
	int diameer=5;
	public void paint(Graphics g)
	{
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0,0,400, 400);
		g.setColor(Color.white);
		g.fillOval(50, 50, diameer, diameer);
	}	
}

//小球会自己动————循环改变小球坐标，用线程休眠控制小球移动速度
class MotionOneself extends JPanel{
	int x=0;
	int y=200;
	int diameer=5;
	//构造初始坐标
	MotionOneself()
	{
		System.out.println("构造小球！");
	}
	public void paint(Graphics g)
	{
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0,400, 400);
		g.setColor(Color.white);
		g.fillOval(x, y, diameer, diameer);
		//调用游戏循环函数，自动改变小球坐标
		this.gameloop();
	}
	//游戏循环
	public void gameloop()
	{
		while(x<300)
		{
			x++;
			System.out.println("小球坐标为"+x);
			//写一个空循环 使当前线程能来得及反应
			for(int i=0;i<1000000000;i++)
			{
				
			}
			this.repaint();
		}
	}
	

}



















