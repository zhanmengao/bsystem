package com.TankGame3;

/*
 * ���ܣ�дһ���ӵ���
 */

public class bullet implements Runnable{
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
	//���캯��
	public bullet(int x,int y,int direct)
	{
		this.x=x;
		this.y=y;
		this.direct=direct;
	}
	//��һ���ƶ�������ÿ��ı�һ���ӵ�����
	public void run() {
		System.out.println("�����ӵ�run");
		//Խ��ʱ�߳�����
		while(isLive==true)
		{
			 System.out.println("�ӵ����꣺x="+x+"y=:"+y);  
			//����ʱ����
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