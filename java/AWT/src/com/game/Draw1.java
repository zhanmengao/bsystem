/*
 * ���ܣ���һ��Բ��һ�����Ρ�һ���߶�
 */
package com.game;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;

import javax.swing.*;

public class Draw1 extends JFrame{
	
	public Draw1()
	{
		MyPanel1 mp=new MyPanel1();
		//�����ˣ����mp��JFrame
		this.add(mp);
		
		//������������
		this.setSize(1200, 750);
		this.setLocation(100, 100);
		this.setTitle("��Һ�����Java");
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	}
	public static void main(String[] args) {
		Draw1 d1=new Draw1();

	}

}
class MyPanel1 extends JPanel{
	//���ػ�ͼ����
	public void paint(Graphics g)//�������g�������ǵĻ���
	{
		//�ǵõ���һ�¸��๹�췽��
		super.paint(g);
		//���濪ʼ��ͼ��������
//		g.drawOval(100, 100, 30, 30);
//		g.drawLine(0, 0, 50, 50);
//		g.drawRect(150, 150, 100, 80);
//		g.setColor(Color.blue);
//		g.fillRect(200, 200, 60, 20);
//		�ڻ����ϻ���ͼƬ
		Image im=Toolkit.getDefaultToolkit().getImage
				(Panel.class.getResource("/Huli.jpg"));
		g.drawImage(im, 0,0, 1200, 750,this);
//		AffineTransform at=new AffineTransform();
//			
//			at.setToTranslation(50, 50);//ƽ��
//			at.rotate(Math.PI/8);//��ת
//			at.setToScale(2, 2);//����
//			at.scale(2, 2);//����
//			
//			Graphics2D g2=(Graphics2D) g;
//			g2.setTransform(at);
		
		//������
//		g.setColor(Color.LIGHT_GRAY);
//		g.setFont(new Font("΢���ź�",Font.BOLD,30));
//		g.drawString("��Ҫѧ��Java��C#", 500, 500);

		
		
	}
	
}