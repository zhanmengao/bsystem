/*
 * 功能：完成坦克大战3.0(主函数部分）
 */
package com.TankGame3;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame3 extends JFrame{
	//开始画前先构造好顶层容器-面板
	MyPanel5 mp2=null;
	
	TankGame3()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
        //启动mp线程  
        Thread t=new Thread(mp2);  
        t.start(); 
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("坦克大战3.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame3 tg2=new TankGame3();
	}
}
//我的画板
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//定义敌人的坦克组
	Vector<Enemy> emy=new Vector<Enemy>();//指定只能返回和接受Enemy类型
	static int tote=3;//定义最多3台敌人坦克
	
	//在画板上定义一个我的坦克
	Hero3 myhero=null;
	public MyPanel5()
	{
		//构造我的坦克
		myhero=new Hero3(50,345,1,1);
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
		if(myhero.bu!=null && myhero.bu.isLive==true)
		{
			g.setColor(Color.white);
			//画子弹(坐标在变，由外界提供）
			g.fillOval(myhero.bu.getX(), myhero.bu.getY(), 2, 2);
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
			this.myhero.move(1);
			//调用向上走的方法
			break;
		case KeyEvent.VK_S:
			this.myhero.setDirect(2);
			this.myhero.move(2);
			//调用向下走的方法
			break;
		case KeyEvent.VK_A:
			this.myhero.setDirect(3);
			this.myhero.move(3);
			//调用向左走的方法
			break;
		case KeyEvent.VK_D:
			this.myhero.setDirect(4);
			this.myhero.move(4);
			//调用向右走的方法 
			break;
	}
	//监听键盘是否按下J键
	if(e.getKeyCode()==KeyEvent.VK_J)
	{
		//调用开火命令
		this.myhero.biubiu();
	}
	this.repaint();	
}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
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
