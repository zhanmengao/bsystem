/*
 * ���ܣ�дһ�����壬��һ����ɫ�����,��һ�º�ɫ����
 */
package com.game;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;


public class Monitor extends JFrame implements ActionListener{
	JPanel mp3=null;
	JButton jb1,jb2;
	Cat mycat=null;
	
	public Monitor()
	{
		mycat=new Cat();
		mp3=new JPanel();mp3.setBackground(Color.orange);
		jb1=new JButton("���ұ��");
		//ע�����
		jb1.addActionListener(this);
		//ָ������ʲô�ַ���
		jb1.setActionCommand("aa");
		jb1.addActionListener(mycat);
		jb1.setActionCommand("aa");
		
		
		jb2=new JButton("���ұ���");
		jb2.addActionListener(this);
		jb2.setActionCommand("bb");
//		jb2.addActionListener(mp3);
		jb2.setActionCommand("bb");
		
		
		
		//�Ѿ���Ĭ�ϵı߽粼��
		
		this.add(jb1,BorderLayout.NORTH);
		this.add(mp3);
		this.add(jb2,BorderLayout.SOUTH);
		
		this.setSize(300, 300);
		this.setLocation(700, 300);
		
		this.setResizable(false);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	

	public static void main(String[] args) {
		Monitor mit=new Monitor();
	}



	public void focusGained(FocusEvent e) {
		// TODO Auto-generated method stub
		
	}



	public void focusLost(FocusEvent e) {
		// TODO Auto-generated method stub
		
	}



	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("aa"))
		{
			System.out.println("������1��");
			mp3.repaint();
			mp3.setBackground(Color.BLACK);
		}
		else if(e.getActionCommand().equals("bb"))
		{
			System.out.println("������2��");
			mp3.repaint();
			mp3.setBackground(Color.green);
		}
				
		
	}

}
class MyPanel3 extends JPanel implements ActionListener{
	public void paint(Graphics g)
	{
		super.paint(g);
		g.setColor(Color.orange);
		g.fillRect(0, 0,300, 300);
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("bb"))
		{
			System.out.println("����Ҳ������");
		}
		
	}


		
}
class Cat implements ActionListener{
	public void actionPerformed(ActionEvent e) {
		if(e.getActionCommand().equals("aa"))
		{
			System.out.println("СèҲ������");
		}
	}
}

