/*
 * 功能：完成坦克大战3.1(主函数部分）
 * 优化：坦克提供了最多发射10颗子弹的功能
 */
package com.TankGame3_1;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame3_1 extends JFrame{
	//开始画前先构造好顶层容器-面板
	MyPanel5 mp2=null;
	
	TankGame3_1()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
        //启动mp线程  
        Thread t=new Thread(mp2);  
        t.start(); 
		
		this.setSize(600,600);
		this.setLocation(700, 300);
		this.setTitle("坦克大战3.1");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame3_1 tg2=new TankGame3_1();
	}
}
//我的画板
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//定义敌人的坦克组
	Vector<Enemy> emy=new Vector<Enemy>();//指定只能返回和接受Enemy类型
	static int tote=5;//定义最多3台敌人坦克
	
	//在画板上定义一个我的坦克
	Hero3_1 myhero=null;
	public MyPanel5()
	{
		//构造我的坦克
		myhero=new Hero3_1(50,545,1,1);
		//构造敌人5级坦克
		for(int i=0;i<tote;i++)
		{
			//构造3个坦克对象，给定初始坐标
			Enemy enemytank=enemytank=new Enemy(60*(i+1),10,2,2);
			emy.add(enemytank);
			//将构造的enemytank向量交给它自己(构造的时候更新Enemy的向量)
			enemytank.setEenemy(emy);
			//初始化的时候启动敌机线程
			Thread t=new Thread(enemytank);
			t.start();
		}	
	}
	//重载paint(g),每100毫秒刷新一次
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 600, 600);	
		//坐标和方向是变量
		if(myhero.isLive==true)
		{
			this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		}
		//遍历坦克集合
		for(int i=0;i<emy.size();i++)
		{
			//用Vector.get(index)，获取emy里的对象信息		
			if(emy.get(i).isLive==true)
			{
				this.drawtank(emy.get(i).getX(),emy.get(i).getY(),g,emy.get(i).getDirect(),emy.get(i).getColor());
			}
			//遍历子弹集合
			for(int j=0;j<emy.get(i).bu.size();j++)
			{
				//从集合里取出子弹
				Bullet3_1 s=emy.get(i).bu.get(j);
				//敌人发射子弹
			    if(emy.get(i).bu.get(j).isLive==true)
			    {
			    	g.setColor(Color.yellow);
			    	//画子弹(坐标在变，由外界提供）
			    	g.fillOval(s.getX(), s.getY(), 2, 2);
			    }
		     }
		}
		for(int i=0;i<myhero.bu.size();i++)
		{
			//取出子弹
			Bullet3_1 bl=myhero.bu.get(i);
			//我发射子弹
			if(bl!=null && bl.isLive==true)
			{
				g.setColor(Color.white);
				//画子弹(坐标在变，由外界提供）
				g.fillOval(bl.getX(), bl.getY(), 2, 2);
			}
		}
	}
	//检查敌人子弹与我的坦克死亡！
	public void EnymybuAndMe()
	{
		//取出每一个敌人
		for(int i=0;i<emy.size();i++)
		{
			Enemy em=emy.get(i);
			//取出该敌人的每一颗子弹
			for(int j=0;j<em.bu.size();j++)
			{
				Bullet3_1 s=em.bu.get(j);
				
				//进行匹配
				if(myhero.isLive==true && em.bu.get(j).isLive==true)
				{
					this.dead(s, myhero);
				}
				//移除敌人子弹
				if(em.bu.get(j).isLive==false)
				{
					em.bu.remove(em.bu.get(j));
				}	
			}
			
		}
	}
	
	//检查我的子弹与敌人坦克死亡！
	public void MybuAndEnemy()
	{
		for(int i=0;i<emy.size();i++)
		{
			Enemy em=emy.get(i);
			for(int j=0;j<myhero.bu.size();j++)
			{
				Bullet3_1 s=myhero.bu.get(j);
				if(myhero.bu.get(j).isLive==true && emy.get(i).isLive==true)
				{
					this.dead(s, em);
				}	
				if(emy.get(i).isLive==false)
				{
					emy.remove(emy.get(i));
				}
				if(myhero.bu.get(j).isLive==false)
				{
					myhero.bu.remove(myhero.bu.get(j));
				}
			}
		}
	}
	
	//判断坦克，子弹的存活（提供一个子弹和一台坦克）
	public void dead(Bullet3_1 bu,Tank3_1 et)
	{
		//1、子弹碰壁时死亡
		if(bu.getX()>599 && bu.getX()<1 ||
				bu.getY()<1 || bu.getY()>599 )
		{
			bu.isLive=false;
		}
		//2、敌人坦克与子弹相撞死亡(判断每个图形与子弹是否接触）
		//bu(x1,y1,w1,h1),et(x2,y2,w2,h2)
		switch(et.direct)
		{
		case 1:
			if((bu.x-et.x+11<5 && et.x-11-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x-6<5 && et.x+6-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x+6<12 && et.x-6-bu.x<12 && bu.y-et.y+5<12 && et.y-5-bu.y<2))
			{
				//死亡
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 2:
			if((bu.x-et.x+11<5 && et.x-11-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x-6<5 && et.x+6-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x+6<12 && et.x-6-bu.x<12 && bu.y-et.y+10<12 && et.y-10-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 3:
			if((bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y+11<5 && et.y-11-bu.y<2)||
					(bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y-6<5 && et.y+6-bu.y<2)||
					(bu.x-et.x-3<12 && et.x+3-bu.x<12 && bu.y-et.y+6<12 && et.y-6-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 4:
			if((bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y+11<5 && et.y-11-bu.y<2)||
					(bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y-6<5 && et.y+6-bu.y<2)||
					(bu.x-et.x+5<12 && et.x+-5-bu.x<12 && bu.y-et.y+6<12 && et.y-6-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
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
		this.myhero.s.dxdy=20;
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
			//每100毫秒检查敌人坦克与我的子弹
			this.MybuAndEnemy();
			this.EnymybuAndMe();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//每100秒重绘
			this.repaint();
		}
	}
}
