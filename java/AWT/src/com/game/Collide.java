/*
 * 功能：实现碰撞检测
 * 思想：当两个物体坐标范围发生交集，则视为碰撞发生
 */
package com.game;
import java.awt.*;
import javax.swing.*;

public class Collide extends JFrame{
	MyBall mb=null;
	public Collide()
	{
		mb=new MyBall();
		
		this.add(mb);
		
		this.setSize(500, 500);
		this.setLocation(500,500);
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String []args)
	{
		Collide co=new Collide();
	}

}
class MyBall extends JPanel implements Runnable{
	//定义两个小球
	Ball ball[]=null;;
	int tote=2;
	MyBall()
	{
		ball=new Ball[tote];
		ball[0]=new Ball(50,50);
		ball[1]=new Ball(10,10);
	}

	public void paint(Graphics g)
	{
		//小球A:红色
		g.setColor(Color.red);
		g.fillOval(ball[0].getX(), ball[0].getY(), ball[0].getDiammer(), ball[0].getDiammer());
		//小球B：蓝色
		g.setColor(Color.blue);
		g.fillOval(ball[1].getX(), ball[1].getY(), ball[1].getDiammer(), ball[1].getDiammer());
		ball[0].goint();
		
	}
	//控制小球移动
	public void move_ball()
	{
		while(true)
		{
			ball[0].setX(ball[0].getX()+3);
			ball[0].setY(ball[0].getY()+5);
			ball[1].setX(ball[1].getX()+7);
			ball[1].setY(ball[1].getY()+1);
			//判断小球是否碰撞
			if(ball[0].getX()-ball[1].getY()<ball[1].getDiammer() || ball[1].getX()-ball[0].getX()<ball[0].getDiammer()
					|| ball[0].getY()-ball[1].getY()<ball[1].getDiammer() || ball[1].getY()-ball[0].getY()<ball[0].getDiammer())
			{
				//发生碰撞,令两个小球反向
				
		   }
	}		
}

	@Override
		public void run() 
		{
			//在线程里改变两个小球的坐标
				try {
					Thread.sleep(100);
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		
	}
	

class Ball {
	//我认为两个小球都有坐标和半径,和速度
	private int x;private int dx=3;
	private int y;private int dy=3;
	private int direct;
	public int getDiammer() {
		return diammer;
	}
	public void setDiammer(int diammer) {
		this.diammer = diammer;
	}
	private int diammer=5;
	public int getX() {
		return x;
	}
	public void setX(int x) {
		this.x = x;
	}
	public int getY() {
		return y;
	}
	public void setY(int y) {
		this.y = y;
	}
	//构造小球
	public Ball(int x,int y)
	{
		this.x=x;
		this.y=y;	
	}
	//小球会运动
	public void goint()
	{
		while(true)
		{
			this.direct=(int)(1+Math.random()*3);
			switch(direct)
			{
			case 1:
				y-=dx;
				break;
			case 2:
				y+=dx;
				break;
			case 3:
				x-=dx;
				break;
			case 4:
				x+=dx;
				break;
			}
		}

	}
}



















