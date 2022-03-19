/*
 *���ܣ���Ϸѭ�������߳�
 *�÷�������Ϸѭ���ŵ�run()���棬��ʵ�����̣߳������߳�
 *Ŀ��:ʹ֡�����ȶ���ÿ����ͼ���л���������һ֡����һ����ֹ�ġ�������Ҳ����һ����Ϸѭ����
 */
package com.game;

public class FrameRate implements Runnable{

	public static void main(String[] args) {
		
	}
	//�����߳�
	public void gameStart()
	{
		FrameRate fr=new FrameRate();
		Thread t1=new Thread(fr);
		t1.start();
	}
	//����������Ҫ��֡����
	private static final int FPS=50;
	//ͨ��end-start��������ϷFPS
	public void run() 
	{
		//��¼һ��ѭ����Ҫ��ʱ��
		long period=1000/FPS;
		//��¼��һ�ν���run��ʱ��(����)
		long t1=System.currentTimeMillis();
		
		while(true)
		{
			//��Ϸ���
			//Loop,Render,Paint
			
			//��¼һ��ѭ��������ʱ��
			long t2=System.currentTimeMillis();
			//��¼������Ҫ˯�ߵ�ʱ�䣨�涨ʱ��-����ʱ�䣩
			long sleepTime=period-(t2-t1);
			//��ȻsleepTime����Ϊ����������Ϊ������ʱ�����ǽ�����ʱ����Ϊ��С��ֵ
			if(sleepTime<0)
			{
				sleepTime=1;
			}
			//�����ṩ������ʱ�� �ó������ߣ�ʹFPS�ȶ�
			try {
				Thread.sleep(sleepTime);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			//���»�ȡʱ��
			t1=System.currentTimeMillis();
			
		}
		
	}
	

}
