/*
 * 功能：实现最简单的线程
 */
package com.game;

public class CopycatThread {
	public static void main(String []args)
	{
		//继承方法启动线程
		Inherit in=new Inherit();
		in.start();
		
		//接口方法启动线程
		Realize re=new Realize();
		Thread th=new Thread(re);
		th.start();
		
	}
}
//方法1、继承Thread类（线程）
class Inherit extends Thread{
	//重写run()方法
	public void run()
	{
		for(int i=0;i<10;i++)
		{
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("hello world!"+(i+1)+"次+来自继承实现");
		}
		
	}
}
//方法2、实现Runnabel接口
class Realize implements Runnable{

	public void run()
	{
		for(int i=0;i<10;i++)
		{
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("hello world!"+(i+1)+"次，来自接口实现");
		}
	}
	
}
