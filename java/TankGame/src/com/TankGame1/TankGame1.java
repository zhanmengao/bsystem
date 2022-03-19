/*
 * 功能：完成坦克大战1.0
 */
package com.TankGame1;
import java.awt.*;
import javax.swing.*;


public class TankGame1 extends JFrame{
	//开始画前先构造好顶层容器-面板
	MyPanel2 mp2=null;
	
	TankGame1()
	{
		mp2=new MyPanel2();
		
		this.add(mp2);
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("坦克大战1.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame1 tg=new TankGame1();
	}

}
//我的画板
class MyPanel2 extends JPanel{
	//在画板上定义一个我的坦克
	Hero myhero=null;
	//在构造方法里创建我的坦克（赋值初始位置）
	public MyPanel2()
	{
		myhero=new Hero(10,10);	
	}
	//重载paint(g)
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);	
		this.drawtank(myhero.getX(), myhero.getY(), g, 1, 1);
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
		switch(direct)
		{
		case 1://如果是1就向上画
			//1、画出左边矩形
			g.fill3DRect(x, y, 5, 20,true);
			//2、画出右边矩形
			g.fill3DRect(x+15, y, 5, 20, true);
			//3、画出中间矩形
			g.fill3DRect(x+5, y+5, 10,15, true);
			//4、画出中间圆形
			g.fillOval(x+4, y+5,10, 10);
			//5、画出炮筒
			g.drawLine(x+9, y+5, x+9, y-2);
		}
		

	}
}

//做一个坦克父类
class Tank{
	int x=0;
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
	int y=0;
	//坦克父类提供一个构造方法 用于设置起始坐标
	public Tank(int x,int y)
	{
		this.x=x;
		this.y=y;
	}
}
class Hero extends Tank{

	//我的坦克继承了父类构造方法，也提供初始坐标，以及一些其它的我自己的属性
	public Hero(int x, int y) {
		super(x, y);	
	}
	
}
