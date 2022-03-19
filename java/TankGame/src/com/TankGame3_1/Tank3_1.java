package com.TankGame3_1;

import java.util.Vector;

/*
 * 功能：坦克类
 */

//定义坦克父类
public class Tank3_1 {
	//Bullet3_1 bu=null;//创建一个子弹集合
	Vector<Bullet3_1> bu=new Vector<Bullet3_1>();
	Bullet3_1 s=null;
	int x=0;
	int y=0;
	int dx=1;
	int dy=1;
	public int direct=0;//用int代表方向
	int color=0;//用int代表颜色
	boolean isLive=true;
	boolean overlap=false;//是否处于碰撞的判定参数，初始为false
	
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
	//坦克父类提供一个构造方法 用于设置起始坐标
	public Tank3_1(int x,int y,int direct,int color)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
		this.color=color;
		
	}
	//提供移动的方法
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
	//提供开火的方法：1、根据方向实例化一个子弹   2、用该子弹启动线程
	public void biubiu()
	{
		if(this.isLive==true){
			//如果当前已经发射了五颗子弹 则不允许开火
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
		//根据坦克方向，给初始化的子弹方向赋值
		//用实例化完成的对象启动子弹线程
		Thread t1=new Thread(s);
		t1.start();
	}
}
