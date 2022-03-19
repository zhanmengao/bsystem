/*
 * 功能：做一个记事本窗体
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class Mean extends JFrame{
	JScrollPane jsp=null;
	JTextArea jta=null;
	//我认为这里有1个菜单条
	JMenuBar jmb=null;
	//一级菜单条里有5个菜单
	JMenu jm1[]=new JMenu[5];
	//其中1，1里面有7个子选项
	JMenuItem []jmi1=new JMenuItem[7];
	
	//其中1,1的新建里面，又有（文件、工程）.故在写一个二级菜单
	JMenuItem wenben=null;
	JMenuItem xiangmu=null;
	
	//工具条
	JToolBar jtb=null;
	JButton jb1[]=new JButton[6];
	
	//构造
	public Mean()
	{
		jta=new JTextArea();
		jsp=new JScrollPane(jta);
		jmb=new JMenuBar();
		
		jm1[0]=new JMenu("文件(F)");
		jm1[0].setMnemonic('F');
		jm1[1]=new JMenu("编辑(E)");
		jm1[1].setMnemonic('E');
		jm1[2]=new JMenu("格式(O)");
		jm1[2].setMnemonic('O');
		jm1[3]=new JMenu("查看(V)");
		jm1[3].setMnemonic('V');
		jm1[4]=new JMenu("帮助(H)");
		jm1[4].setMnemonic('H');
		
		for(int i=0;i<jm1.length;i++)
		{
			jmb.add(jm1[i]);
		}
		//初始化树叶
		jmi1[0]=new JMenu("新建");
		jmi1[1]=new JMenuItem("打开");
		jmi1[2]=new JMenuItem("保存");
		jmi1[3]=new JMenuItem("另存为");
		jmi1[4]=new JMenuItem("页面设置");
		jmi1[5]=new JMenuItem("打印");
		jmi1[6]=new JMenuItem("退出");
		
		//再给树叶（新建），加入二级树叶（文本，项目）
		wenben=new JMenuItem("文本");
		xiangmu=new JMenuItem("项目");
		jmi1[0].add(wenben);
		jmi1[0].add(xiangmu);
		//把树叶添加到所属的树枝
		for(int i=0;i<jmi1.length;i++)
		{
			jm1[0].add(jmi1[i]);
			if(i==3 || i==5)
			{
				//添加分割线
				jm1[0].addSeparator();
			}
		}

		
		jtb=new JToolBar();
		jb1[0]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[0].setToolTipText("新建");//设置提示信息
		jb1[1]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[1].setToolTipText("保存");//设置提示信息
		jb1[2]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[2].setToolTipText("打开");//设置提示信息
		jb1[3]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[4]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[5]=new JButton(new ImageIcon("image/Dogs.jpg"));
		for(int i=0;i<jb1.length;i++)
		{
			jtb.add(jb1[i]);
		}
		
		//将菜单条添加到窗体上(setJMenuBar)
		this.setJMenuBar(jmb);		
		//将工具条添加到窗体上
		this.add(jtb,BorderLayout.NORTH);
		
		JScrollPane jsp=new JScrollPane(jta);
		jsp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		this.add(jsp);
		
		this.setLocation(500, 300);
		this.setSize(900,600);
		this.setTitle("记事本");
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}
	

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Mean m=new Mean();
	}

}
