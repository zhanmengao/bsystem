package com.TankGame3;

/*
 * 功能：写一个子弹类
 */

public class bullet implements Runnable{
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
	int dxdy=10;
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
	public bullet(int x,int y,int direct)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
	}
	//做一个移动方法，每秒改变一次子弹坐标
	public void run() {
		System.out.println("进入子弹run");
		//越界时线程死亡
		while(isLive==true)
		{
			 System.out.println("子弹坐标：x="+x+"y=:"+y);  
			//碰壁时死亡
			if(x>399 || x<1 || y<1 || y>399)
			{
				this.isLive=false;
				break;
			}
			try {
				Thread.sleep(100);
			} catch (Exception e) {
				// TODO: handle exception
			}
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