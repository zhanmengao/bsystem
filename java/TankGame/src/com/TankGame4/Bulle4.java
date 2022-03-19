package com.TankGame4;

/*
 * 功能：写一个子弹类
 */

public class Bulle4 implements Runnable{
	//我认为子弹有坐标、方向、速度
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
	int x=0;
	int y=0;
	int direct;
	int dxdy=7;
	public int getDxdy() {
		return dxdy;
	}
	public void setDxdy(int dxdy) {
		this.dxdy = dxdy;
	}
	boolean isLive=true;
	public int getDirect() {
		return direct;
	}
	public void setDirect(int direct) {
		this.direct = direct;
	}
	public boolean getisLive() {
		return isLive;
	}
	public void setLive(boolean isLive) {
		this.isLive = isLive;
	}
	//构造函数
	public Bulle4(int x,int y,int direct)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
	}
	//做一个移动方法，每秒改变一次子弹坐标
	public void run() {
		//子弹死亡时线程结束
		while(isLive==true)
		{
			try {
				Thread.sleep(100);
			} catch (Exception e) {
				
			}
			//当前处于暂停，则子弹停止运动
			if(MyPanel5.isPaused==false)
			{
			switch(direct)
			{
			case 1:
				y-=dxdy;
				break;
			case 2:
				y+=dxdy;
				break;
			case 3:
				x-=dxdy;
				break;
			case 4:
				x+=dxdy;
				break;
			}
			}
		}
		}

	}