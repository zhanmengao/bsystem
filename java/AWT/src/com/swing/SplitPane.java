/*
 * 功能：拆分窗格 JSplitPane
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class SplitPane extends JFrame {
	//我认为这个窗体需要1个JLabel和1个JList，1个JSplitPane
	JSplitPane jsplitpane1;
	JLabel jlabel1;
	JList jlist1;

	
	SplitPane()
	{
		//先定义JList（内容为姿势名）和JLabe（内容为图片），
		String[] ox={"传教士式","后入式","老汉推车","女上式"};
		jlist1=new JList(ox);
		jlabel1=new JLabel(new ImageIcon("images/IMG_1410.JPG"));
		jsplitpane1=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,jlist1,jlabel1);
		jsplitpane1.setOneTouchExpandable(true);//允许拉伸
		
		//设置布局
		this.setLayout(new BorderLayout());
		
		//添加组件
		this.add(jsplitpane1);
		
		//设置窗体参量
		this.setSize(900, 1000);
		this.setLocation(600, 300);
		
		//设置显示 关闭 不可拉伸
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}

	public static void main(String[] args) {
		SplitPane sp=new SplitPane();

	}

}
