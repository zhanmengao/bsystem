/*
 * 计算器
 */
package com.swing;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.*;
//我认为这个上部分应为空布局，下部分应为网格布局，整体是空布局
//1、实现监听到加减乘除法，输出栏能输出结果

public class View extends JFrame implements ActionListener{
	ArrayList equ=new ArrayList();
	//上部分应该由2个JPanel，3个JButton，还有一张图片面板组成
	JPanel jp1=null;
	JPanel jp2=null;
	MyPanelView mpv=null;
	JButton jb1[]=new JButton[3];
	
	//下部分由20个JButton组成
	JPanel jp3=null;
	JButton jb2[]=new JButton[20];
	
	View()
	{
		this.setLayout(null);
		//先初始化上部
		jp1=new JPanel();
		mpv=new MyPanelView();
		jb1[0]=new JButton("←");
		jb1[1]=new JButton("CE");
		jb1[2]=new JButton("C");
		//上部JPanel为空布局
		jp1.setLayout(null);
		//设置图片在JPanel上的位置
		mpv.setBounds(25, 20,300, 60);
		jp1.add(mpv);
		//三个JButton→放在一个网格布局的JPanel上
		jp2=new JPanel();
		jp2.setLayout(new GridLayout(1,3,20,10));
		for(int i=0;i<jb1.length;i++)
		{
		jp2.add(jb1[i]);
		}
		jp2.setBounds(30, 100, 270, 30);
		jp1.add(jp2);
		jp1.setBounds(0, 0, 350, 130);
		this.add(jp1);
		
		//下面是20格的网格布局
		jp3=new JPanel();
		jp3.setLayout(new GridLayout(4,5,5,10));
		
		jb2[0]=new JButton("7");
		jb2[0].addActionListener(this);
		jb2[0].setActionCommand("7");
		
		jb2[1]=new JButton("8");
		jb2[1].addActionListener(this);
		jb2[1].setActionCommand("8");
		
		jb2[2]=new JButton("9");
		jb2[2].addActionListener(this);
		jb2[2].setActionCommand("9");
		
		jb2[3]=new JButton("/");
		jb2[3].addActionListener(this);
		jb2[3].setActionCommand("/");
		
		jb2[4]=new JButton("sqrt");
		jb2[4].addActionListener(this);
		jb2[4].setActionCommand("sqrt");
		
		jb2[5]=new JButton("4");
		jb2[5].addActionListener(this);
		jb2[5].setActionCommand("4");
		
		jb2[6]=new JButton("5");
		jb2[6].addActionListener(this);
		jb2[6].setActionCommand("5");
		
		jb2[7]=new JButton("6");
		jb2[7].addActionListener(this);
		jb2[7].setActionCommand("6");
		
		jb2[8]=new JButton("×");
		jb2[8].addActionListener(this);
		jb2[8].setActionCommand("×");
		
		jb2[9]=new JButton("%");
		jb2[9].addActionListener(this);
		jb2[9].setActionCommand("%");
		
		jb2[10]=new JButton("1");
		jb2[10].addActionListener(this);
		jb2[10].setActionCommand("1");
		
		jb2[11]=new JButton("2");
		jb2[11].addActionListener(this);
		jb2[11].setActionCommand("2");
		
		jb2[12]=new JButton("3");
		jb2[12].addActionListener(this);
		jb2[12].setActionCommand("3");
		
		jb2[13]=new JButton("-");
		jb2[13].addActionListener(this);
		jb2[13].setActionCommand("-");
		
		jb2[14]=new JButton("1/x");
		jb2[14].addActionListener(this);
		jb2[14].setActionCommand("1/x");
		
		jb2[15]=new JButton("0");
		jb2[15].addActionListener(this);
		jb2[15].setActionCommand("0");
		
		jb2[16]=new JButton("+-");
		jb2[16].addActionListener(this);
		jb2[16].setActionCommand("+-");
		
		jb2[17]=new JButton(".");
		jb2[17].addActionListener(this);
		jb2[17].setActionCommand(".");
		
		jb2[18]=new JButton("+");
		jb2[18].addActionListener(this);
		jb2[18].setActionCommand("+");
		
		jb2[19]=new JButton("?");
		jb2[19].addActionListener(this);
		jb2[19].setActionCommand("?");
		
		for(int i=0;i<jb2.length;i++)
		{
			
			jp3.add(jb2[i]);
		}
		jp3.setBounds(0, 160, 350, 170);
		this.add(jp3);
		
		this.setSize(355,395);
		this.setLocation(500, 500);
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		View v=new View();
	}

	@Override
	public void actionPerformed(ActionEvent e) {
		int sum=0;
		String a=e.getActionCommand();
		
		
		
		
		
		
	}

}
class MyPanelView extends JPanel{
	public void paint(Graphics g)
	{
		Image im=Toolkit.getDefaultToolkit().getImage(Panel.class.getResource("/计算器2.png"));
		g.drawImage(im, 0, 0, 300,60 ,this);
	}
}






