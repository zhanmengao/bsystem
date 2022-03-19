/*
 * 功能：JTextField文本框，JPasswordField密码框，JLabel标签
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class Field extends JFrame{
	//定义组件，我认为有3个JPanel，一个JTextField和JPasswordField，两个JLabel，两个JButton
	JPanel jp[]=new JPanel[3];
	JTextField jtext1;
	JPasswordField jpass1;
	JLabel []jla=new JLabel[2];
	JButton []jbu=new JButton[2];
	
	Field()
	{
		//初始化组件
		jp[0]=new JPanel();jp[1]=new JPanel();jp[2]=new JPanel();
		jtext1=new JTextField(10);
		jpass1=new JPasswordField(10);
		jla[0]=new JLabel("用户名");
		jla[1]=new JLabel("密    码");
		jbu[0]=new JButton("确认");jbu[1]=new JButton("取消");
		
		//添加组件
		jp[0].add(jla[0]);jp[0].add(jtext1);
		jp[1].add(jla[1]);jp[1].add(jpass1);
		jp[2].add(jbu[0]);jp[2].add(jbu[1]);
		for(int i=0;i<jp.length;i++)
		{
			this.add(jp[i]);
		}
		
		//设置布局、窗体属性
		this.setLayout(new GridLayout(3,1));
		this.setSize(300,200);
		this.setLocation(700, 350);
		this.setTitle("教务管理系统");
		
		//设置显示、关闭、不可拉伸
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}

	public static void main(String[] args) {
		Field fi=new Field();
	}

}
