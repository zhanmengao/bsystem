/*
 * 功能：完成坦克大战2.0
 */
package com.TankGame2;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame2 extends JFrame{
	//开始画前先构造好顶层容器-面板
	MyPanel5 mp2=null;
	
	TankGame2()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("坦克大战2.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame2 tg2=new TankGame2();
	}

}
//我的画板
class MyPanel5 extends JPanel implements KeyListener{
	//定义敌人的坦克组
	Vector<Enemy> emy=new Vector<Enemy>();//指定只能返回和接受Enemy类型
	static int tote=3;//定义最多3台敌人坦克
	
	
	//在画板上定义一个我的坦克
	Hero2 myhero=null;
	public MyPanel5()
	{
		//构造我的坦克
		myhero=new Hero2(50,345,1,1);
		//构造敌人5级坦克
		for(int i=0;i<tote;i++)
		{
			//构造3个坦克对象，给定初始坐标(第一个坦克）
			Enemy enemytank=enemytank=new Enemy(30,10,2,2);
			//把5级坦克添加到集合
			emy.add(enemytank);
		}
	}
	//重载paint(g)
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);	
		//坐标和方向是变量
		this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		//画出i个敌人5级坦克，并制定方向（变量）
		for(int i=0;i<tote;i++)
		{
			//用Vector.get(index)，获取emy里的对象信息
			this.drawtank((emy.get(i).getX()+1)*(i+1),emy.get(i).getY(),g,emy.get(i).getDirect(),emy.get(i).getColor());
		}
		
	}
	
	//写一个画坦克的函数：需要传坦克当前位置，坦克行走方向，坦克类型
	public void drawtank(int x,int y,Graphics g,int direct,int type)
	{
		//根据坦克类型判断颜色
		switch(type)
		{
		case 1://我方坦克
			g.setColor(Color.CYAN);
			break;
		case 2://敌方5级坦克
			g.setColor(Color.yellow);
			break;
		} 
		//方向
		switch(direct)
		{
		//如果是1就向上画
				case 1:
					//1、画出左边矩形
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2、画出右边矩形
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3、画出中间矩形
					g.fill3DRect(x-6, y-5, 12,12, true);
					//4、画出中间圆形
					g.fillOval(x-5, y-4,8,8);
					//5、画出炮筒
					g.drawLine(x, y, x, y-12);
					break;
				//是2就向下画
				case 2:
					//1、画出左边矩形
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2、画出右边矩形
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3、画出中间矩形
					g.fill3DRect(x-6, y-10, 12,12, true);
					//4、画出中间圆形
					g.fillOval(x-5, y-7,8,8);
					//5、画出炮筒
					g.drawLine(x, y, x, y+8);
					break;
				//3就往左走
				case 3:
					//1、画出左边矩形
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2、画出右边矩形
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3、画出中间矩形
					g.fill3DRect(x+3, y-6, 12,12, true);
					//4、画出中间圆形
					//g.fillOval(x-4, y-4,8,8);
					//5、画出炮筒
					g.drawLine(x+7, y, x-8, y);
					break;
				case 4:
					//1、画出左边矩形
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2、画出右边矩形
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3、画出中间矩形
					g.fill3DRect(x-5, y-6, 12,12, true);
					//4、画出中间圆形
					g.fillOval(x-4, y-4,8,8);
					//5、画出炮筒
					g.drawLine(x, y, x+15, y);
					break;
		}

	}
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	//处理键盘按下这件事情,w上s下a左d右
	public void keyPressed(KeyEvent e) {
		int keycode=e.getKeyCode();
		switch(keycode)
		{
		case KeyEvent.VK_W:
			this.myhero.setDirect(1);
			this.myhero.moveUP();
			//调用向上走的方法
			break;
		case KeyEvent.VK_S:
			this.myhero.setDirect(2);
			this.myhero.moveDOWN();
			//调用向下走的方法
			break;
		case KeyEvent.VK_A:
			this.myhero.setDirect(3);
			this.myhero.moveLEFT();
			//调用向左走的方法
			break;
		case KeyEvent.VK_D:
			this.myhero.setDirect(4);
			this.myhero.moveRIGHT();
			//调用向右走的方法 
			break;
	}
	repaint();	
	}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
}

//做一个坦克父类
class Tank2{
	int x=0;
	int y=0;
	int dx=1;
	int dy=1;
	int direct=0;//用int代表方向
	int color=0;//用int代表颜色
	public int getDirect() {
		return direct;
	}
	public void setDirect(int direct) {
		this.direct = direct;
	}
	public int getColor() {
		return color;
	}
	public void setColor(int color) {
		this.color = color;
	}

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

	//坦克父类提供一个构造方法 用于设置起始坐标
	public Tank2(int x,int y,int direct,int color)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
		this.color=color;
	}

}
class Hero2 extends Tank2{
	//我的坦克继承了父类构造方法，也提供初始坐标，以及一些其它的我自己的属性
	public Hero2(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=1;
		dy=2;
	}
	//提供移动的方法
	void moveUP()
	{
		y-=dy;
	}
	void moveDOWN()
	{
		y+=dy;
	}
    void moveLEFT()
	{
		x-=dx;
	}
    void moveRIGHT()
	{
		x+=dx;
	}
	
}
class Enemy extends Tank2
{

	public Enemy(int x, int y, int direct, int color) {
		super(x, y, direct, color);
	}

}
