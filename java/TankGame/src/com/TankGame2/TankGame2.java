/*
 * ���ܣ����̹�˴�ս2.0
 */
package com.TankGame2;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame2 extends JFrame{
	//��ʼ��ǰ�ȹ���ö�������-���
	MyPanel5 mp2=null;
	
	TankGame2()
	{
		mp2=new MyPanel5();
		this.addKeyListener(mp2);
		
		this.add(mp2);
		
		this.setSize(400,400);
		this.setLocation(500, 300);
		this.setTitle("̹�˴�ս2.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame2 tg2=new TankGame2();
	}

}
//�ҵĻ���
class MyPanel5 extends JPanel implements KeyListener{
	//������˵�̹����
	Vector<Enemy> emy=new Vector<Enemy>();//ָ��ֻ�ܷ��غͽ���Enemy����
	static int tote=3;//�������3̨����̹��
	
	
	//�ڻ����϶���һ���ҵ�̹��
	Hero2 myhero=null;
	public MyPanel5()
	{
		//�����ҵ�̹��
		myhero=new Hero2(50,345,1,1);
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
			this.myhero.moveUP();
			//���������ߵķ���
			break;
		case KeyEvent.VK_S:
			this.myhero.setDirect(2);
			this.myhero.moveDOWN();
			//���������ߵķ���
			break;
		case KeyEvent.VK_A:
			this.myhero.setDirect(3);
			this.myhero.moveLEFT();
			//���������ߵķ���
			break;
		case KeyEvent.VK_D:
			this.myhero.setDirect(4);
			this.myhero.moveRIGHT();
			//���������ߵķ��� 
			break;
	}
	repaint();	
	}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
}

//��һ��̹�˸���
class Tank2{
	int x=0;
	int y=0;
	int dx=1;
	int dy=1;
	int direct=0;//��int������
	int color=0;//��int������ɫ
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

	//̹�˸����ṩһ�����췽�� ����������ʼ����
	public Tank2(int x,int y,int direct,int color)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
		this.color=color;
	}

}
class Hero2 extends Tank2{
	//�ҵ�̹�˼̳��˸��๹�췽����Ҳ�ṩ��ʼ���꣬�Լ�һЩ���������Լ�������
	public Hero2(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=1;
		dy=2;
	}
	//�ṩ�ƶ��ķ���
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
