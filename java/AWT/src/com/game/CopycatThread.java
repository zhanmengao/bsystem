/*
 * ���ܣ�ʵ����򵥵��߳�
 */
package com.game;

public class CopycatThread {
	public static void main(String []args)
	{
		//�̳з��������߳�
		Inherit in=new Inherit();
		in.start();
		
		//�ӿڷ��������߳�
		Realize re=new Realize();
		Thread th=new Thread(re);
		th.start();
		
	}
}
//����1���̳�Thread�ࣨ�̣߳�
class Inherit extends Thread{
	//��дrun()����
	public void run()
	{
		for(int i=0;i<10;i++)
		{
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			System.out.println("hello world!"+(i+1)+"��+���Լ̳�ʵ��");
		}
		
	}
}
//����2��ʵ��Runnabel�ӿ�
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
			System.out.println("hello world!"+(i+1)+"�Σ����Խӿ�ʵ��");
		}
	}
	
}
