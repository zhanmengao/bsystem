/*
 * 功能：完成坦克大战3.1(主函数部分）
 * 优化：坦克提供了最多发射10颗子弹的功能
 */
package com.TankGame4;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame4 extends JFrame implements KeyListener,ActionListener{
	//开始画前先构造好顶层容器-面板
	MyPanel5 mp2=null;
	int xlength=630;
	public int getXlength() {
		return xlength;
	}
	public void setXlength(int xlength) {
		this.xlength = xlength;
	}
	public int getYlength() {
		return ylength;
	}
	public void setYlength(int ylength) {
		this.ylength = ylength;
	}
	int ylength=600;
	
	GamePrepare gp=null;
	
	//定义菜单
	JMenuBar jmb=null;
	JMenu[] jm=new JMenu[5];
	JMenuItem []jmi=new JMenuItem[4];
	
	TankGame4()
	{
		//初始化树干、树枝
		jmb=new JMenuBar();
		jm[0]=new JMenu("游戏");
		//初始化树叶
		jmi[0]=new JMenuItem("新游戏");
		jmi[1]=new JMenuItem("保存");
		jmi[2]=new JMenuItem("读取");
		jmi[3]=new JMenuItem("退出");
		jmb.add(jm[0]);
		for(int i=0;i<jmi.length;i++)
		{
			jm[0].add(jmi[i]);
			jmi[i].addActionListener(this);
		}
		this.setJMenuBar(jmb);
		//给新游戏 注册监听，委托JFrame类办事
		jmi[0].setActionCommand("StartGame");
		jmi[1].setActionCommand("save");
		jmi[2].setActionCommand("read");
		jmi[3].setActionCommand("exit");
		
		//把开始界面添加到JFrame
		gp=new GamePrepare();
		this.add(gp);
		//启动 gp线程（闪烁）
		Thread t1=new Thread(gp);
		t1.start();
		//注册监听；让它自己做监听者
		this.addKeyListener(this);
		
		this.setSize(xlength,ylength);
		this.setLocation(700, 300);
		this.setTitle("坦克大战4.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame4 tg2=new TankGame4();
	}
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void keyPressed(KeyEvent e) {
		//按下空格：开始/暂停
		if(e.getKeyCode()==KeyEvent.VK_SPACE)
		{
				MyPanel5.isPaused=!MyPanel5.isPaused;
		}
		
		
	}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		//如果按了开始游戏菜单
		String s=e.getActionCommand();
		if(s.equals("StartGame"))
		{
			AePlayWave a=new AePlayWave("C:\\CloudMusic\\群星 - FC《坦克大战》战前BGM.mp3");
			a.start();
			//删除旧面板
			this.remove(gp);
			mp2=new MyPanel5(false);
			this.add(mp2);
			//设置新面板显示
			this.setVisible(true);
			this.addKeyListener(mp2);
	        //启动mp线程  
	        Thread t=new Thread(mp2);  
	        t.start(); 
		}else if(s.equals("save"))
		{
			System.out.println("保存");
			//在点击保存的时候，把向量传给记录类。让他去写
			new Record().setV(mp2.emy);
			//保存当前总分到文件	
			Record.Write();
			
			
			
		}else if(s.equals("read"))
		{
			
			System.out.println("读取");
			//删除旧面板
			this.remove(gp);
			mp2=new MyPanel5(true);
			this.add(mp2);
			//设置新面板显示
			this.setVisible(true);
			this.addKeyListener(mp2);
	        //启动mp线程  
	        Thread t=new Thread(mp2);  
	        t.start();
		}
		else if(s.equals("exit"))
		{
			System.out.println("退出");
			System.exit(0);
		}
	}
	

}
//我的画板（准备界面）
class GamePrepare extends JPanel implements Runnable{
	static int time=0;
	int i=1;
	public void paint(Graphics g)
	{
		super.paint(g);
		//用一个矩形，上面画字符串代表准备界面
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 500, 400);
		//绘制字符串
		g.setColor(Color.RED);
		Font myfont=new Font("华文新魏",Font.BOLD,30);
		g.setFont(myfont);
		 //做成闪烁效果（每0.5秒改变一次time值）
		if(time%2==0)
		{
			g.drawString("第"+(i)+"关", 200, 200);
		}
	}

	@Override
	public void run() {
		while(true)
		{
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//每0.5秒重绘，前0.5秒显示String，后0.5秒不会显示
		repaint();
		time++;
		}
	}
}
//我的画板（游戏面板）
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//暂停的布尔
	static boolean isPaused=false;
	//定义敌人的坦克组
	Vector<Enemy> emy=new Vector<Enemy>();//指定只能返回和接受Enemy类型
	static int tote=5;//定义最多5台敌人坦克
	
	//在画板上定义一个我的坦克
	Hero4 myhero=null;
	public MyPanel5(boolean read)
	{
		//看是否读取
		if(read==false)
		{
			Record.get_total();
			//构造我的坦克
			myhero=new Hero4(50,345,1,1);
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
		else if(read==true)
		{
			//让MyPanel知道记录到的坐标
			Vector<Node> node=new Vector<Node>();
			node=Record.getNode();
			//构造我的坦克
			myhero=new Hero4(50,345,1,1);
			//构造敌人5级坦克
			for(int i=0;i<node.size();i++)
			{
				Node no=node.get(i);
				//构造3个坦克对象，给定初始坐标
				Enemy enemytank=enemytank=new Enemy(no.x,no.y,no.direct,2);
				emy.add(enemytank);
				//将构造的enemytank向量交给它自己(构造的时候更新Enemy的向量)
				enemytank.setEenemy(emy);
				//初始化的时候启动敌机线程
				Thread t=new Thread(enemytank);
				t.start();
				}
		}
	}
	//准备一个画提示信息的函数
	public void ShowRecord(Graphics g)
	{
		//画出记录坦克
		//首先是敌人
		this.drawtank(80, 450, g, 1, 2);
		g.setColor(Color.black);
		g.setFont(new Font("微软雅黑",Font.BOLD,20));
		g.drawString(""+Record.getResidue(), 100, 450);
			
		//画自己
		this.drawtank(200, 450, g, 1, 1);
		g.setColor(Color.black);
		g.drawString(Record.getMylife()+"", 220, 450);
		
		//画出右边计分
		g.drawString("您的总分",510, 50);
		g.drawString(Record.getTotal()+"", 510, 100);
		g.drawString("击杀", 510, 150);
		this.drawtank(520, 180, g, 1, 2);
		g.setColor(Color.black);
		g.drawString("×"+Record.getKill(), 535, 186);

	}
	//重载paint(g),每100毫秒刷新一次
	public void paint(Graphics g)
	{
		super.paint(g);
		//恢复总分
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 500,400);	
		this.ShowRecord(g);
		//坐标和方向是变量
		if(myhero.isLive==true)
		{
			this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		}
		//遍历坦克集合
		for(int i=0;i<emy.size();i++)
		{
			Enemy enemytank=emy.get(i);
			//用Vector.get(index)，获取emy里的对象信息		
			if(emy.get(i).isLive==true)
			{
				this.drawtank(enemytank.getX(),enemytank.getY(),g,enemytank.getDirect(),enemytank.getColor());
			}
			//遍历子弹集合
			for(int j=0;j<enemytank.bu.size();j++)
			{
				//从集合里取出子弹
				Bulle4 s=enemytank.bu.get(j);
				//敌人发射子弹
			    if(s.isLive==true)
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
			Bulle4 bl=myhero.bu.get(i);
			//我发射子弹
			if(bl!=null && bl.isLive==true)
			{
				g.setColor(Color.white);
				//画子弹(坐标在变，由外界提供）
				g.fillOval(bl.getX(), bl.getY(), 2, 2);
			}
		}
	}
	//如果敌人坦克总额没有结束，则再构造
	public void creatEnemy()
	{
		//如果总量大于0
		while((Record.getResidue()-emy.size())>0 && tote-emy.size()>0)
		{
			boolean b=true;
			int ranx=30+(int)(Math.random()*(150-30));
			int rany=30+(int)(Math.random()*(150-30));
			//如果不与已有坦克重叠（如果有一个重叠就算重叠）
			for(int i=0;i<emy.size();i++)
			{
				if(!(ranx-emy.get(i).x>30) && !(emy.get(i).x-ranx>30)
						&& !(rany-emy.get(i).y>30) && !(emy.get(i).y-rany>30))
				{
					b=false;
				}
			}
			//该位置可以创建
			if(b==true)
				{
				//构造新的坦克，数量是屏幕总量-屏幕存活
				for(int j=0;j<tote-emy.size();j++)
				{
					Enemy enemytank=new Enemy(ranx,rany,2,2);
					emy.add(enemytank);
					//将构造的enemytank向量交给它自己(构造的时候更新Enemy的向量)
					enemytank.setEenemy(emy);
					//初始化的时候启动敌机线程
					Thread t=new Thread(enemytank);
					t.start();
				}
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
				Bulle4 s=em.bu.get(j);
				
				//进行匹配
				if(myhero.isLive==true && em.bu.get(j).isLive==true)
				{
					this.dead(s, myhero);
				}
				//（如果死亡）移除敌人子弹
				if(em.bu.get(j).isLive==false)
				{
					em.bu.remove(em.bu.get(j));
				}
				if(myhero.isLive==false)
				{
					Record.killme();
					//如果还有命
					if(Record.getMylife()>0)
					{
						//构造我的坦克
						myhero=new Hero4(50,345,1,1);
					}
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
				Bulle4 s=myhero.bu.get(j);
				//如果存活，则判断位置
				if(s.isLive==true && em.isLive==true)
				{
					this.dead(s, em);
				}
				//如果死亡，则移除敌人
				if(em.isLive==false)
				{
					emy.remove(em);
					Record.killenemy();
				}
				//如果死亡，则移除我的子弹
				if(s.isLive==false)
				{
					myhero.bu.remove(s);
				}
			}
		}
	}
	
	//判断坦克，子弹的存活（提供一个子弹和一台坦克）
	public void dead(Bulle4 bu,Tank4 et)
	{
		//1、子弹碰壁时死亡
		if(bu.getX()>499 && bu.getX()<1 ||
				bu.getY()<1 || bu.getY()>399 )
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
		//如果不处于暂停，监听才有效
		if(isPaused==false)
		{
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
}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	//控制我的画板更新页面
	public void run() 
	{
		int time=0;
		while(true)
		{
			if(isPaused==false)
			{
			//每10毫秒检查子弹和坦克
			this.MybuAndEnemy();
			this.EnymybuAndMe();
			if(time%200==0)
			{
				this.creatEnemy();
			}
			time++;
			}
			//每10秒重绘
			this.repaint();
			try {
				Thread.sleep(5);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
