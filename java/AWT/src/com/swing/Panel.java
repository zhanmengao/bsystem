/*
 * ����:��JPanelдһ��ƻ�����ϴ���
 * 1���̳�JFrame�������java.awt.*��javax.swing.*;
 * 2��������Ҫ�����
 * 3���ڹ��췽���ﴴ�����
 * 4��������
 * 5������JFrame����
 * 6������JFrame��ʾ���رա���������
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class Panel extends JFrame{
	JPanel jp[]=new JPanel[2];
	JButton jb[]=new JButton[5]; 
	
	Panel()
	{
		//��ʼ�����
		jp[0]=new JPanel();
		jp[1]=new JPanel();
		jb[0]=new JButton("����");
		jb[1]=new JButton("ƻ��");
		jb[2]=new JButton("�㽶");
		jb[3]=new JButton("ˮ����");
		jb[4]=new JButton("����");
		
		//������
		jp[0].add(jb[1]);
		jp[0].add(jb[2]);
		jp[1].add(jb[3]);
		jp[1].add(jb[4]);
		this.add(jp[0],BorderLayout.NORTH);
		this.add(jp[1],BorderLayout.SOUTH);
		this.add(jb[0]);
		
		//���ô�������
		this.setSize(300,300);;
		this.setLocation(700,350);
		this.setTitle("������ˮ��һ��");
		
		//���ô�����ʾ��������ر�
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		Panel pa=new Panel();
	}
	

}
