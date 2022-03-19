/*
 * 功能:用JPanel写一个苹果西瓜窗体
 * 1、继承JFrame，引入包java.awt.*包javax.swing.*;
 * 2、定义需要的组件
 * 3、在构造方法里创建组件
 * 4、添加组件
 * 5、设置JFrame属性
 * 6、设置JFrame显示、关闭、不可缩放
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class Panel extends JFrame{
	JPanel jp[]=new JPanel[2];
	JButton jb[]=new JButton[5]; 
	
	Panel()
	{
		//初始化组件
		jp[0]=new JPanel();
		jp[1]=new JPanel();
		jb[0]=new JButton("西瓜");
		jb[1]=new JButton("苹果");
		jb[2]=new JButton("香蕉");
		jb[3]=new JButton("水蜜桃");
		jb[4]=new JButton("龙眼");
		
		//添加组件
		jp[0].add(jb[1]);
		jp[0].add(jb[2]);
		jp[1].add(jb[3]);
		jp[1].add(jb[4]);
		this.add(jp[0],BorderLayout.NORTH);
		this.add(jp[1],BorderLayout.SOUTH);
		this.add(jb[0]);
		
		//设置窗体属性
		this.setSize(300,300);;
		this.setLocation(700,350);
		this.setTitle("我们是水果一家");
		
		//设置窗体显示不可拉伸关闭
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		Panel pa=new Panel();
	}
	

}
