package com.TankGame3_1;


/*
 * 功能：我的坦克类
 */
public class Hero3_1 extends Tank3_1{
	
	
	//我的坦克继承了父类构造方法，也提供初始坐标，以及一些其它的我自己的属性
	public Hero3_1(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=3;
		dy=3;
	}
}









