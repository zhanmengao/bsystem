package com.TankGame3_1;

import java.util.Vector;

//敌人坦克也能移动和发射子弹
//坦克一直在重绘，但是方向不需要一直重绘（5秒重绘一次方向）
//因为碰撞时敌人坦克自身的行为，所以我认为应该把碰撞写到自身的方法里
public class Enemy extends Tank3_1 implements Runnable
{

	
	public Enemy(int x, int y, int direct, int color) {
		super(x, y, direct, color);
	}
	
	Vector<Enemy> ve=new Vector<Enemy>();
	//获取MyPanel上的敌人坦克
	public void setEenemy(Vector<Enemy> en)
	{
		this.ve=en;
	}
	//处理它们的碰撞
	public void notoverlap()
	{
		//用获取的坦克与其他坦克进行比较
		for(int i=0;i<ve.size();i++)
		{
			//首先和它比较的不是它本身
			if(this!=ve.get(i))
			{
				//再判定是否碰撞（满足条件表示碰了，进入）
				if((this.x-ve.get(i).x<22) && (ve.get(i).x-this.x<22)
						&& (this.y-ve.get(i).y<22) && (ve.get(i).y-this.y<22))
				{
					switch(this.direct)
					{
					case 1:
						this.direct=2;
						break;
					case 2:
						this.direct=1;
						break;
					case 3:
						this.direct=4;
						break;
					case 4:
						this.direct=3;
						break;
						
						
					}
				}
			}
		}


	}
	public void run() {
		//每4秒改变一次坦克方向：方法：生成一个随机数（1~4），分别代表上下左右移动
		int time=0;
		while(true)
		{
			//每次线程检查敌人坦克是否相撞
			this.notoverlap();
			//每3.5秒重新选择一个方向
			if(time==0||time%35==0)
			{
				this.direct=(int)(1+Math.random()*3);
				this.overlap=false;
			}
			//敌人坦克自动发射子弹
			if(time==0||time%40==0)
			{
				this.biubiu();
			}
			//如果坦克死亡 退出线程
			if(this.isLive==false)
			{
				break;
			}
			//敌人坦克移动(如果敌人坦克没有相撞)
			if(this.overlap==false && this.overlap==false)
			{
				this.move(this.direct);
			}	
			//敌人坦克迷途知返:因为炮筒最突出，我认为只需要判断炮筒最前面没有碰到边界	
			switch(this.direct)
			{
			case 1:
				if(this.y-13<0)
				{
					this.direct=2;
				}
			    break;
			case 2:
				if(this.y+13>600)
				{
					this.direct=1;
				}
			    break;

			case 3:
				if(this.x-7<0)
					{
					this.direct=4;
					}
				break;
			case 4:
				if(this.x+7>600)
					{
					this.direct=3;
					}
				break;    
			}
			time++;
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}	
	}		
}
