/*
 * 图书查看器
 */
package com.swing;
import java.awt.*;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
//分析：左边应为6*2的网格布局，右边空布局。整体1*2的网格布局

public class Book extends JFrame implements ActionListener{
	//左边6个标签，6个单行文本框
	JLabel []jla=new JLabel[2];//给窗体顶层制造距离
	JPanel jp1=null;
	JLabel jl1[]=new JLabel[6];
	JTextField jtf1[]=new JTextField[7];
	
	//右边：7个JButton,一个JLabel（显示图片）
	JPanel jp2=null;
	JPanel jp3=null;
	JButton jb1[]=new JButton[6];
	JLabel jb2=null;
	
	
	Book()
	{
		this.setLayout(new GridLayout(1,2,20,20));
		//先构造左边
		jp1=new JPanel();
		jp1.setLayout(new GridLayout(7,2,10,30));
		//用于与窗体顶层制造距离
		jla[0]=new JLabel(" ");jla[1]=new JLabel(" ");
		jp1.add(jla[0]);jp1.add(jla[1]);
		
		jl1[0]=new JLabel("书编号",JLabel.CENTER);
		jl1[1]=new JLabel("书名",JLabel.CENTER);
		jl1[2]=new JLabel("书作者",JLabel.CENTER);
		jl1[3]=new JLabel("出版社",JLabel.CENTER);
		jl1[4]=new JLabel("状态",JLabel.CENTER);
		jl1[5]=new JLabel("已售本数",JLabel.CENTER);
		for(int i=0;i<jtf1.length;i++)
		{
			jtf1[i]=new JTextField(10);
		}
		for(int i = 0;i<jl1.length;i++)
		{
			jp1.add(jl1[i]);
			jp1.add(jtf1[i]);
		}
		this.add(jp1);
		
		//再构造右边：我认为是网格布局，主JPanel jp2上面有两个JPanel,mpp和jp3
		jp2=new JPanel();
		jp3=new JPanel();
		jp2.setLayout(new GridLayout(2,1,50,50));
		//上部分是一张图片
		MyPanelPicture mpp=new MyPanelPicture();
		jp2.add(mpp);
		
		jp3.setLayout(new GridLayout(3,2,50,50));
		jb1[0]=new JButton("上一本");
		jb1[1]=new JButton("下一本");
		jb1[2]=new JButton("添加");
		jb1[3]=new JButton("保存");
		jb1[4]=new JButton("删除");
		jb1[5]=new JButton("关闭");
		for(int i=0;i<jb1.length;i++)
		{
			jp3.add(jb1[i]);
		}
		jp2.add(jp3);
		
		this.add(jp2);
		this.setSize(600,400);
		this.setLocation(300, 300);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
		this.setResizable(false);
	}
	public static void main(String []args)
	{
		Book p=new Book();
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		
	}
}
class MyPanelPicture extends JPanel{
	public void paint(Graphics g)
	{
		super.paint(g);
		Image im=Toolkit.getDefaultToolkit().getImage
				(Panel.class.getResource("/Huli.jpg"));
		g.drawImage(im, 0,10, 300, 187,this);
	}
}





