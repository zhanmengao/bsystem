package com.TankGame3;

/*
 * 功能：坦克子类（我方坦克，敌方坦克）
 */
public class Hero3 extends Tank3{
	
	bullet bu=null;//创建一个子弹对象
	
	//我的坦克继承了父类构造方法，也提供初始坐标，以及一些其它的我自己的属性
	public Hero3(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=2;
		dy=2;
	}
	
	//提供开火的方法：1、根据方向实例化一个子弹   2、用该子弹启动线程
	public void biubiu()
	{
		System.out.println("开火！");
		//根据坦克方向，给初始化的子弹方向赋值
		switch(this.direct)
		{
		case 1:
			bu=new bullet(this.x,this.y-12,1);
			break;
		case 2:
			bu=new bullet(this.x,this.y+8,2);
			break;
		case 3:
			bu=new bullet(this.x-8,this.y,3);
			break;
		case 4:
			bu=new bullet(this.x+15,this.y,4);
			break;
		}
		//用实例化完成的对象启动子弹线程
		Thread t1=new Thread(bu);
		t1.start();

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
}
class Enemy extends Tank3
{

	public Enemy(int x, int y, int direct, int color) {
		super(x, y, direct, color);
	}

}

