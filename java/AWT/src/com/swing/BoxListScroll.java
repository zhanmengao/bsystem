/*
 * 功能：下拉菜单JComboBox 列表框JList 滚动窗格JScrollPane
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class BoxListScroll extends JFrame{
	//我认为JPanel有2个，JComboBox有1个，JList有1个,JLabel有2个,以及一个滚动窗格
	JPanel jp1,jp2;
	JComboBox jcb;//注意整一个下拉菜单算一个组件
	JList jli;//注意整一个列表框算一个组件
	JLabel []jla=new JLabel[2];
	JScrollPane jsp;
	
	BoxListScroll()
	{
		//初始化组件
		jp1=new JPanel();jp2=new JPanel();
		jla[0]=new JLabel("您的籍贯是");
		jla[1]=new JLabel("您想要去的旅游地点是");
		
		String[] aa={"北京","上海","天津","火星"};//用字符数组初始化JComboBox
		jcb=new JComboBox(aa);	
		
		String[] dd={"长城","东方明珠","海河","什么什么"};//用字符数组初始化JList
		jli=new JList(dd);//JList不具备拉伸功能
		jli.setVisibleRowCount(3);
		jsp=new JScrollPane(jli);//将列表框JList添加到滚动窗格，再把滚动窗格插入到JPanel
		
		//添加组件
		jp1.add(jla[0]);jp1.add(jcb);
		jp2.add(jla[1]);jp2.add(jsp);//注意是添加滚动窗格到JPanel
		this.add(jp1);
		this.add(jp2);
		
		//设置窗体属性
		this.setLayout(new GridLayout(2,1));
		this.setSize(300,200);
		this.setLocation(700,300);
		this.setTitle("小调查");
		
		//设置窗体显示，不可拉伸，关闭
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
	}
	
	public static void main(String[] args) {

		BoxListScroll bls=new BoxListScroll();
	}

}
