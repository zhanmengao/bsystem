package com.TankGame3_1;

/*
 * ���ܣ�дһ���ӵ���
 */

public class Bullet3_1 implements Runnable{
	//����Ϊ�ӵ������ꡢ�����ٶ�
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
	//���캯��
	public Bullet3_1(int x,int y,int direct)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
	}
	//��һ���ƶ�������ÿ��ı�һ���ӵ�����
	public void run() {
		//�ӵ�����ʱ�߳̽���
		while(isLive==true)
		{
			try {
				Thread.sleep(100);
			} catch (Exception e) {
				
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