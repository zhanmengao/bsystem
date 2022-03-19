/*
 *功能：实现售票系统的对象锁 
 */
package com.game;

public class Blockade {

	public static void main(String[] args) {
		ShopPicket sp=new ShopPicket();
		//定义3个线程对象，同时操作一个数据
		Thread t1=new Thread(sp);
		Thread t2=new Thread(sp);
		Thread t3=new Thread(sp);
		
		t1.start();
		t2.start();
		t3.start();
	}

}
//作一个总售票点
class ShopPicket implements Runnable{
	private int num=2000;
	
	public void run() {
		synchronized(this)
		{
			while(num>0)
			{
				System.out.println(Thread.currentThread().getName()+"售出了第"+(2000-num)+"一张票");
				num--;
				try {
					Thread.sleep(10);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}

	}

	
}
