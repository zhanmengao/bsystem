/*
 * 功能：复选框JCheckBox,单选框JRadioButton,单选框组ButtonGroup
 */
package com.swing;
import java.awt.*;
import javax.swing.*;;

public class Box extends JFrame{
	//我认为需要3个JPanel，3个JCheckBox，2个JRadioButton，2个JLael，2个JButton
	JPanel jp[]=new JPanel[3];
	JCheckBox jcb[]=new JCheckBox[3];
	JRadioButton jrb[]=new JRadioButton[2];
	JLabel []jla=new JLabel[2];
	JButton []jbu=new JButton[2];
	ButtonGroup bg;
	
	Box()
	{
		//复选框内容为篮球足球网球，单选框内容为男/女
		jp[0]=new JPanel();jp[1]=new JPanel();jp[2]=new JPanel();
		
		jla[0]=new JLabel("你喜欢的运动是");
		jcb[0]=new JCheckBox("篮球");jcb[1]=new JCheckBox("足球");jcb[2]=new JCheckBox("网球");
		
		jla[1]=new JLabel("你的性别是");
		jrb[0]=new JRadioButton("男");jrb[1]=new JRadioButton("女");
		bg=new ButtonGroup();bg.add(jrb[0]);bg.add(jrb[1]);//添加单选框到单选框组
		
		
		
		jbu[0]=new JButton("注册用户");jbu[1]=new JButton("取消注册");
		
		//把组件添加
		jp[0].add(jla[0]);
		for(int i=0;i<jcb.length;i++)
		{
			jp[0].add(jcb[i]);
		}
		
		jp[1].add(jla[1]);
		for(int i=0;i<jrb.length;i++)
		{
			jp[1].add(jrb[i]);
		}
		
		for(int i=0;i<jbu.length;i++)
		{
			jp[2].add(jbu[i]);
		}
		
		for(int i=0;i<jp.length;i++)
		{
			this.add(jp[i]);
		}
		
		
		//设置窗体属性
		this.setLayout(new GridLayout(3,1));
		this.setSize(400,200);
		this.setLocation(700, 360);
		this.setTitle("用户注册界面");
		
		//设置窗体显示、禁止拉伸、关闭
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		Box bo=new Box();
	}

}
