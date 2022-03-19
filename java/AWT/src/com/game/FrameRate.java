/*
 *功能：游戏循环引入线程
 *用法：将游戏循环放到run()里面，再实例化线程，启动线程
 *目的:使帧速率稳定（每秒钟图形切换的数量，一帧就是一幅静止的————也就是一次游戏循环）
 */
package com.game;

public class FrameRate implements Runnable{

	public static void main(String[] args) {
		
	}
	//启动线程
	public void gameStart()
	{
		FrameRate fr=new FrameRate();
		Thread t1=new Thread(fr);
		t1.start();
	}
	//设置我们想要的帧速率
	private static final int FPS=50;
	//通过end-start，控制游戏FPS
	public void run() 
	{
		//记录一次循环需要的时间
		long period=1000/FPS;
		//记录第一次进入run的时间(毫秒)
		long t1=System.currentTimeMillis();
		
		while(true)
		{
			//游戏框架
			//Loop,Render,Paint
			
			//记录一次循环结束的时间
			long t2=System.currentTimeMillis();
			//记录本次需要睡眠的时间（规定时间-消耗时间）
			long sleepTime=period-(t2-t1);
			//当然sleepTime可能为负数，当它为负数的时候我们将休眠时间设为较小的值
			if(sleepTime<0)
			{
				sleepTime=1;
			}
			//根据提供的休眠时间 让程序休眠，使FPS稳定
			try {
				Thread.sleep(sleepTime);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			//重新获取时间
			t1=System.currentTimeMillis();
			
		}
		
	}
	

}
