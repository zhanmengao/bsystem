/*
 * ���ܣ�������Ϸѭ��
 * �÷�����ѭ��
 * ���ã�ʹ�߳��ܽ��յ�ÿһ������
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
//С���Ǿ�ֹ�ġ�����������ֵ���ı�
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

//С����Լ�����������ѭ���ı�С�����꣬���߳����߿���С���ƶ��ٶ�
class MotionOneself extends JPanel{
	int x=0;
	int y=200;
	int diameer=5;
	//�����ʼ����
	MotionOneself()
	{
		System.out.println("����С��");
	}
	public void paint(Graphics g)
	{
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0,400, 400);
		g.setColor(Color.white);
		g.fillOval(x, y, diameer, diameer);
		//������Ϸѭ���������Զ��ı�С������
		this.gameloop();
	}
	//��Ϸѭ��
	public void gameloop()
	{
		while(x<300)
		{
			x++;
			System.out.println("С������Ϊ"+x);
			//дһ����ѭ�� ʹ��ǰ�߳������ü���Ӧ
			for(int i=0;i<1000000000;i++)
			{
				
			}
			this.repaint();
		}
	}
	

}



















