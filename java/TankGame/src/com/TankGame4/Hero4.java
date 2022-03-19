package com.TankGame4;


/*
 * 功能：我的坦克类
 */
public class Hero4 extends Tank4{
	
	
	//我的坦克继承了父类构造方法，也提供初始坐标，以及一些其它的我自己的属性
	public Hero4(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=3;
		dy=3;
	}
}









