/*
 * ���ܣ����̹�˴�ս3.0(���������֣�
 */
package com.TankGame3;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame3 extends JFrame{
	//��ʼ��ǰ�ȹ���ö�������-���
	MyPanel5 mp2=null;
	
	TankGame3()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
        //����mp�߳�  
        Thread t=new Thread(mp2);  
        t.start(); 
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("̹�˴�ս3.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame3 tg2=new TankGame3();
	}
}
//�ҵĻ���
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//������˵�̹����
	Vector<Enemy> emy=new Vector<Enemy>();//ָ��ֻ�ܷ��غͽ���Enemy����
	static int tote=3;//�������3̨����̹��
	
	//�ڻ����϶���һ���ҵ�̹��
	Hero3 myhero=null;
	public MyPanel5()
	{
		//�����ҵ�̹��
		myhero=new Hero3(50,345,1,1);
		//�������5��̹��
		for(int i=0;i<tote;i++)
		{
			//����3��̹�˶��󣬸�����ʼ����(��һ��̹�ˣ�
			Enemy enemytank=enemytank=new Enemy(30,10,2,2);
			//��5��̹����ӵ�����
			emy.add(enemytank);
		}
	}
	//����paint(g)
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);	
		//����ͷ����Ǳ���
		this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		//����i������5��̹�ˣ����ƶ����򣨱�����
		for(int i=0;i<tote;i++)
		{
			//��Vector.get(index)����ȡemy��Ķ�����Ϣ
			this.drawtank((emy.get(i).getX()+1)*(i+1),emy.get(i).getY(),g,emy.get(i).getDirect(),emy.get(i).getColor());
		}
		if(myhero.bu!=null && myhero.bu.isLive==true)
		{
			g.setColor(Color.white);
			//���ӵ�(�����ڱ䣬������ṩ��
			g.fillOval(myhero.bu.getX(), myhero.bu.getY(), 2, 2);
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
