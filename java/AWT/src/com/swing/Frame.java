/*
 * 功能：练习JFrame,JButton,BorderLayout,FlowLayout,GridLayout的使用
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class Frame extends JFrame{
	//新建组件
	int size=5;
	JButton jb[]=new JButton[size];
	
	Frame()
	{
		//初始化组件
		jb[0]=new JButton("天下如棋，一步三算");
		jb[1]=new JButton("赵子龙，参见！");
		jb[2]=new JButton("生命与信念，都交托阁下！");
		jb[3]=new JButton("放马过来吧！");
		jb[4]=new JButton("大河之剑天上来！");
		
		//边界布局的添加
		this.add(jb[0],BorderLayout.CENTER);
		this.add(jb[1],BorderLayout.NORTH);
		this.add(jb[2],BorderLayout.SOUTH);
		this.add(jb[3],BorderLayout.WEST);
		this.add(jb[4],BorderLayout.EAST);
	
		//添加组件,流失布局和网格布局都这样
		/*
		 for(int i=0;i<5;i++)
		{
			this.add(jb[i]);
		}
		*/
		
		//设置JFrame布局、属性
		//this.setLayout(new BorderLayout());
//		this.setLayout(new FlowLayout(FlowLayout.LEFT));
		//this.setLayout(new GridLayout(5,1,10,10));
		this.setSize(500,500);
		this.setLocation(300, 500);
		this.setTitle("王者荣耀");
		
		//设置JFrame不可拉伸、显示、关闭形式
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
	}

	public static void main(String[] args) {
		Frame fr=new Frame();
	}

}
