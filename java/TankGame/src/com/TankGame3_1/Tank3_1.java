package com.TankGame3_1;

import java.util.Vector;

/*
 * ���ܣ�̹����
 */

//����̹�˸���
public class Tank3_1 {
	//Bullet3_1 bu=null;//����һ���ӵ�����
	Vector<Bullet3_1> bu=new Vector<Bullet3_1>();
	Bullet3_1 s=null;
	int x=0;
	int y=0;
	int dx=1;
	int dy=1;
	public int direct=0;//��int������
	int color=0;//��int������ɫ
	boolean isLive=true;
	boolean overlap=false;//�Ƿ�����ײ���ж���������ʼΪfalse
	
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
	public Tank3_1(int x,int y,int direct,int color)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
		this.color=color;
		
	}
	//�ṩ�ƶ��ķ���
	public void move(int direct)
	{
		switch(direct)
		{
		case 1:
			y-=dy;
			break;
		case 2:
			y+=dy;
			break;
		case 3:
			x-=dx;
			break;
		case 4:
			x+=dx;
			break;
		}
	}
	//�ṩ����ķ�����1�����ݷ���ʵ����һ���ӵ�   2���ø��ӵ������߳�
	public void biubiu()
	{
		if(this.isLive==true){
			//�����ǰ�Ѿ�����������ӵ� ��������
			if(bu.size()<=5)
			{
				switch(this.direct)
				{
				case 1:
					s=new Bullet3_1(this.x,this.y-12,1);
					bu.add(s);
					break;
				case 2:
					s=new Bullet3_1(this.x,this.y+8,2);
					bu.add(s);
					break;
				case 3:
					s=new Bullet3_1(this.x-8,this.y,3);
					bu.add(s);
					break;
				case 4:
					s=new Bullet3_1(this.x+15,this.y,4);
					bu.add(s);
					break;
				}
			}
		}	
		//����̹�˷��򣬸���ʼ�����ӵ�����ֵ
		//��ʵ������ɵĶ��������ӵ��߳�
		Thread t1=new Thread(s);
		t1.start();
	}
}
