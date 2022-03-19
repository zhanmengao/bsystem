/*
 * ���ܣ�ʹ��˫������ƻ�ͼ���������ơ�ǰ����ʾ��
 * ���ã���ֹ������˸
 */
package com.game;

import javax.swing.*;
import java.awt.*;

public class DoubleBuffer extends JFrame{
	MyPanela mpa=null;
	public DoubleBuffer()
	{
		mpa=new MyPanela();
		this.add(mpa);

		
		this.setSize(400,400);
		this.setLocation(500, 500);
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	public static void main(String[] args) {
		DoubleBuffer db=new DoubleBuffer();
	}
}
class MyPanela extends JPanel{
	Image im=null;
	//��ͼ����
	private int x=0;
	private int y=0;
	private int diameer=5;

	
	public void paint(Graphics g)
	{
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 400, 400);
		while(x<300)
		{
			this.gameLoop();
			this.gameRender();
			this.gamepaint();
		}

	}
	//������Ϸ�߼�
	public void gameLoop()
	{
		x+=5;
	}
	//��������
	public void gameRender()
	{
		System.out.println("������������");
		//�������ڴ�С��Image����
		Image im=createImage(400,400);
		//��ȡim��Graphics����
		Graphics dbg=im.getGraphics();
		//���û�����ɫ
		dbg.setColor(Color.white);
		//��һ��С��
		dbg.fillOval(x, 200, diameer, diameer);
		System.out.println("�滭��ϣ�");
	}
	//ǰ����ʾ
	public void gamepaint()
	{
		System.out.println("����ǰ����ʾ");
		Graphics g=this.getGraphics();
		//��ͼ�񻺳��������ݸ��Ƶ�ǰ��������
		g.drawImage(im,0,0,null);
		//�ͷŵ�ǰ��Ļ��Graphics����
		g.dispose();
		System.out.println("������������ǰ����");
	}

	
	
	
	
	
	
	
}
