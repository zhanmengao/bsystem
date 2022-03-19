/*
 * 功能：JTextArea多行文本框
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class TextArea extends JFrame{
	//我认为这里有1个JTextArea，1个JComboBox,1个JTextField，1个JButton
	JPanel jpanel1=null;
	JTextArea jtextarea1=null;JScrollPane jscrollpane1=null;
	JComboBox jcombobox1=null;
	JTextField jtextfield1=null;
	JButton jbutton1=null;
	
	TextArea()
	{
		//初始化 下拉菜单内容为聊天人的姓名，按钮内容为发送
		jpanel1=new JPanel();
		jtextarea1=new JTextArea();
		jscrollpane1=new JScrollPane(jtextarea1);//将多行文本框添加到滚动窗格，实现可以上下滑动
		
		String friends[]={"兔子","呶呶","呶呶","白白"};
		jcombobox1=new JComboBox(friends);
		jtextfield1=new JTextField(10);
		jbutton1=new JButton("发送");
		
		//设置布局
		this.setLayout(new BorderLayout());
		
		//添加组件
		jpanel1.add(jcombobox1);
		jpanel1.add(jtextfield1);
		jpanel1.add(jbutton1);
		this.add(jscrollpane1);
		this.add(jpanel1,BorderLayout.SOUTH);

		
		//设置窗体属性
		this.setSize(500, 300);
		this.setLocation(500, 500);
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		this.setTitle("聊天中");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {

		TextArea ta=new TextArea();
	}

}
