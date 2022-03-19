/*
 * ���ܣ����̹�˴�ս3.1(���������֣�
 * �Ż���̹���ṩ����෢��10���ӵ��Ĺ���
 */
package com.TankGame3_1;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame3_1 extends JFrame{
	//��ʼ��ǰ�ȹ���ö�������-���
	MyPanel5 mp2=null;
	
	TankGame3_1()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
        //����mp�߳�  
        Thread t=new Thread(mp2);  
        t.start(); 
		
		this.setSize(600,600);
		this.setLocation(700, 300);
		this.setTitle("̹�˴�ս3.1");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame3_1 tg2=new TankGame3_1();
	}
}
//�ҵĻ���
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//������˵�̹����
	Vector<Enemy> emy=new Vector<Enemy>();//ָ��ֻ�ܷ��غͽ���Enemy����
	static int tote=5;//�������3̨����̹��
	
	//�ڻ����϶���һ���ҵ�̹��
	Hero3_1 myhero=null;
	public MyPanel5()
	{
		//�����ҵ�̹��
		myhero=new Hero3_1(50,545,1,1);
		//�������5��̹��
		for(int i=0;i<tote;i++)
		{
			//����3��̹�˶��󣬸�����ʼ����
			Enemy enemytank=enemytank=new Enemy(60*(i+1),10,2,2);
			emy.add(enemytank);
			//�������enemytank�����������Լ�(�����ʱ�����Enemy������)
			enemytank.setEenemy(emy);
			//��ʼ����ʱ�������л��߳�
			Thread t=new Thread(enemytank);
			t.start();
		}	
	}
	//����paint(g),ÿ100����ˢ��һ��
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 600, 600);	
		//����ͷ����Ǳ���
		if(myhero.isLive==true)
		{
			this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		}
		//����̹�˼���
		for(int i=0;i<emy.size();i++)
		{
			//��Vector.get(index)����ȡemy��Ķ�����Ϣ		
			if(emy.get(i).isLive==true)
			{
				this.drawtank(emy.get(i).getX(),emy.get(i).getY(),g,emy.get(i).getDirect(),emy.get(i).getColor());
			}
			//�����ӵ�����
			for(int j=0;j<emy.get(i).bu.size();j++)
			{
				//�Ӽ�����ȡ���ӵ�
				Bullet3_1 s=emy.get(i).bu.get(j);
				//���˷����ӵ�
			    if(emy.get(i).bu.get(j).isLive==true)
			    {
			    	g.setColor(Color.yellow);
			    	//���ӵ�(�����ڱ䣬������ṩ��
			    	g.fillOval(s.getX(), s.getY(), 2, 2);
			    }
		     }
		}
		for(int i=0;i<myhero.bu.size();i++)
		{
			//ȡ���ӵ�
			Bullet3_1 bl=myhero.bu.get(i);
			//�ҷ����ӵ�
			if(bl!=null && bl.isLive==true)
			{
				g.setColor(Color.white);
				//���ӵ�(�����ڱ䣬������ṩ��
				g.fillOval(bl.getX(), bl.getY(), 2, 2);
			}
		}
	}
	//�������ӵ����ҵ�̹��������
	public void EnymybuAndMe()
	{
		//ȡ��ÿһ������
		for(int i=0;i<emy.size();i++)
		{
			Enemy em=emy.get(i);
			//ȡ���õ��˵�ÿһ���ӵ�
			for(int j=0;j<em.bu.size();j++)
			{
				Bullet3_1 s=em.bu.get(j);
				
				//����ƥ��
				if(myhero.isLive==true && em.bu.get(j).isLive==true)
				{
					this.dead(s, myhero);
				}
				//�Ƴ������ӵ�
				if(em.bu.get(j).isLive==false)
				{
					em.bu.remove(em.bu.get(j));
				}	
			}
			
		}
	}
	
	//����ҵ��ӵ������̹��������
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
	
	//�ж�̹�ˣ��ӵ��Ĵ��ṩһ���ӵ���һ̨̹�ˣ�
	public void dead(Bullet3_1 bu,Tank3_1 et)
	{
		//1���ӵ�����ʱ����
		if(bu.getX()>599 && bu.getX()<1 ||
				bu.getY()<1 || bu.getY()>599 )
		{
			bu.isLive=false;
		}
		//2������̹�����ӵ���ײ����(�ж�ÿ��ͼ�����ӵ��Ƿ�Ӵ���
		//bu(x1,y1,w1,h1),et(x2,y2,w2,h2)
		switch(et.direct)
		{
		case 1:
			if((bu.x-et.x+11<5 && et.x-11-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x-6<5 && et.x+6-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x+6<12 && et.x-6-bu.x<12 && bu.y-et.y+5<12 && et.y-5-bu.y<2))
			{
				//����
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

	//дһ����̹�˵ĺ�������Ҫ��̹�˵�ǰλ�ã�̹�����߷���̹������
	public void drawtank(int x,int y,Graphics g,int direct,int type)
	{
		//����̹�������ж���ɫ
		switch(type)
		{
		case 1://�ҷ�̹��
			g.setColor(Color.CYAN);
			break;
		case 2://�з�5��̹��
			g.setColor(Color.yellow);
			break;
		} 
		//����
		switch(direct)
		{
		//�����1�����ϻ�
				case 1:
					//1��������߾���
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2�������ұ߾���
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3�������м����
					g.fill3DRect(x-6, y-5, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-5, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x, y-12);
					break;
				//��2�����»�
				case 2:
					//1��������߾���
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2�������ұ߾���
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3�������м����
					g.fill3DRect(x-6, y-10, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-5, y-7,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x, y+8);
					break;
				//3��������
				case 3:
					//1��������߾���
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2�������ұ߾���
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3�������м����
					g.fill3DRect(x+3, y-6, 12,12, true);
					//4�������м�Բ��
					//g.fillOval(x-4, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x+7, y, x-8, y);
					break;
				case 4:
					//1��������߾���
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2�������ұ߾���
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3�������м����
					g.fill3DRect(x-5, y-6, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-4, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x+15, y);
					break;
		}

	}
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	//������̰����������,w��s��a��d��
	public void keyPressed(KeyEvent e) {
		int keycode=e.getKeyCode();
		switch(keycode)
		{
		case KeyEvent.VK_W:
			this.myhero.setDirect(1);
			this.myhero.move(1);
			//���������ߵķ���
			break;
		case KeyEvent.VK_S:
			this.myhero.setDirect(2);
			this.myhero.move(2);
			//���������ߵķ���
			break;
		case KeyEvent.VK_A:
			this.myhero.setDirect(3);
			this.myhero.move(3);
			//���������ߵķ���
			break;
		case KeyEvent.VK_D:
			this.myhero.setDirect(4);
			this.myhero.move(4);
			//���������ߵķ��� 
			break;
	}
	//���������Ƿ���J��
	if(e.getKeyCode()==KeyEvent.VK_J)
	{
		//���ÿ�������
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
	//�����ҵĻ������ҳ��
	public void run() 
	{
		while(true)
		{
			//ÿ100���������̹�����ҵ��ӵ�
			this.MybuAndEnemy();
			this.EnymybuAndMe();
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			//ÿ100���ػ�
			this.repaint();
		}
	}
}
