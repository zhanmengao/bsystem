package com.TankGame3;

/*
 * 功能：坦克类
 */

//定义坦克父类
public class Tank3 {
	int x=0;
	int y=0;
	int dx=1;
	int dy=1;
	int direct=0;//用int代表方向
	int color=0;//用int代表颜色
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
	public Tank3(int x,int y,int direct,int color)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
		this.color=color;
	}
}
