/*
 *���ܣ�ʵ����Ʊϵͳ�Ķ����� 
 */
package com.game;

public class Blockade {

	public static void main(String[] args) {
		ShopPicket sp=new ShopPicket();
		//����3���̶߳���ͬʱ����һ������
		Thread t1=new Thread(sp);
		Thread t2=new Thread(sp);
		Thread t3=new Thread(sp);
		
		t1.start();
		t2.start();
		t3.start();
	}

}
//��һ������Ʊ��
class ShopPicket implements Runnable{
	private int num=2000;
	
	public void run() {
		synchronized(this)
		{
			while(num>0)
			{
				System.out.println(Thread.currentThread().getName()+"�۳��˵�"+(2000-num)+"һ��Ʊ");
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
