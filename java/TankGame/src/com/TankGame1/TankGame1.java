/*
 * ���ܣ����̹�˴�ս1.0
 */
package com.TankGame1;
import java.awt.*;
import javax.swing.*;


public class TankGame1 extends JFrame{
	//��ʼ��ǰ�ȹ���ö�������-���
	MyPanel2 mp2=null;
	
	TankGame1()
	{
		mp2=new MyPanel2();
		
		this.add(mp2);
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("̹�˴�ս1.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame1 tg=new TankGame1();
	}

}
//�ҵĻ���
class MyPanel2 extends JPanel{
	//�ڻ����϶���һ���ҵ�̹��
	Hero myhero=null;
	//�ڹ��췽���ﴴ���ҵ�̹�ˣ���ֵ��ʼλ�ã�
	public MyPanel2()
	{
		myhero=new Hero(10,10);	
	}
	//����paint(g)
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);	
		this.drawtank(myhero.getX(), myhero.getY(), g, 1, 1);
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
		switch(direct)
		{
		case 1://�����1�����ϻ�
			//1��������߾���
			g.fill3DRect(x, y, 5, 20,true);
			//2�������ұ߾���
			g.fill3DRect(x+15, y, 5, 20, true);
			//3�������м����
			g.fill3DRect(x+5, y+5, 10,15, true);
			//4�������м�Բ��
			g.fillOval(x+4, y+5,10, 10);
			//5��������Ͳ
			g.drawLine(x+9, y+5, x+9, y-2);
		}
		

	}
}

//��һ��̹�˸���
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
	//̹�˸����ṩһ�����췽�� ����������ʼ����
	public Tank(int x,int y)
	{
		this.x=x;
		this.y=y;
	}
}
class Hero extends Tank{

	//�ҵ�̹�˼̳��˸��๹�췽����Ҳ�ṩ��ʼ���꣬�Լ�һЩ���������Լ�������
	public Hero(int x, int y) {
		super(x, y);	
	}
	
}
