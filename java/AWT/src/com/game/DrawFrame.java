/*
 * ���ܣ�дһ����ͼ���
 */
package com.game;
import java.awt.*;
import javax.swing.*;

public class DrawFrame extends JFrame{
	//�Ϲ�أ����췽������ҵĻ���Ķ�����ӵ�JFrame
	public DrawFrame()
	{
		//ʵ���������������ҵĻ��壬���ҵĻ�����ӵ���������
	
		MyPanel mp=new MyPanel();
		this.add(mp);
		
		this.setSize(30, 30);
		this.setLocation(0, 0);
		
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		
	}
	public static void main(String[] args) {
		DrawFrame df=new DrawFrame();
	}

}
class MyPanel extends JPanel{
	//���ػ�ͼ����
	public void paint(Graphics g)//�������g�������ǵĻ���
	{
		//�ǵõ���һ�¸��๹�췽��
		super.paint(g);
		//���濪ʼ��ͼ��������
	}
	
}
