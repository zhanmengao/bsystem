/*
 * 功能：空布局
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class NullLayout extends JFrame{
	JButton []jb=new JButton[2];
	public NullLayout()
	{
		jb[0]=new JButton("按钮1");
		jb[1]=new JButton("按钮2");
		//指定按钮的位置和大小
		jb[0].setBounds(50, 50, 100, 50);
		jb[1].setBounds(200,200,100,50);
		for(int i=0;i<jb.length;i++)
		{
			this.add(jb[i]);
		}
		
		
		//设置为空布局
		this.setLayout(null);
		this.setTitle("空布局");
		this.setSize(500,450);
		this.setLocation(0, 0);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String []args)
	{
		NullLayout nl=new NullLayout();
	}
	

}
