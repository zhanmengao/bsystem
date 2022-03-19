/*
 * 功能:页签组件 JTabbedPane:将标签、按钮添加到页签里
 */
package com.swing;
import javax.swing.*;
import java.awt.*;
public class Login extends JFrame{
	//我认为有1个JTabbedPane,6个JLabel，1个JTextField,1个JPasswordField,2个JCheckBox,3个JButton
	//先定义北部,北部是一个JLabel
	JLabel jlab1=null;
	
	//定义南部，是一个JPanel上放了2个按钮
	JPanel jp2;
	JButton jb1,jb2;
	
	//定义中部，是1个JTabbedPane，5个JLabel,2个JCheckBox，1个JTextField,1个JPasswordField,1个JButton
	JPanel jp1,jp3,jp4;
	JTabbedPane jtp;
	JLabel jla1,jla2,jla3,jla4;//QQ号码,QQ密码，找回密码，申请密码保护
	JCheckBox jcb1,jcb2;//记住密码，自动登录
	JTextField jtf;
	JPasswordField jpwf;
	JButton jb3;//注册账号
	
	Login()
	{
		//北部
		jlab1=new JLabel(new ImageIcon("images/CAT.jpg"));
		
		//南部
		jp2=new JPanel();
		jb1=new JButton("登录");
		jb2=new JButton("取消");
		
		//中部
		jp1=new JPanel();
		jp3=new JPanel();
		jp4=new JPanel();
		jp1.setLayout(new GridLayout(3,3));
		//jtp=new JTabbedPane();
		jla1=new JLabel("QQ号码",JLabel.CENTER);
		jla1.setFont(new Font("宋体",Font.PLAIN,16));//设置字体样式
		jtf=new JTextField(10);
		jb3=new JButton("注册账号");jb3.setForeground(Color.blue);
		
		jla2=new JLabel("QQ密码",JLabel.CENTER);
		jla2.setFont(new Font("宋体",Font.PLAIN,16));//设置字体样式
		jpwf=new JPasswordField(10);
		jla3=new JLabel("找回密码",JLabel.CENTER);jla3.setForeground(Color.pink);//设置字体颜色
		jla3.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));//设置鼠标移到上面会变成手
		
		jcb1=new JCheckBox("记住密码");
		jcb2=new JCheckBox("自动登录");
		jla4=new JLabel("申请密码保护",JLabel.CENTER);jla4.setForeground(Color.red);//设置字体颜色
		jla4.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));//设置鼠标移到上面会变成手		
		
		//设置布局
		this.setLayout(new BorderLayout());
		
		//添加组件
		this.add(jlab1,BorderLayout.NORTH);
		
	    //jp1.add(jtp);
		//jp1.setBackground(Color.red);
		jp1.add(jla1);jp1.add(jtf);jp1.add(jb3);
		jp1.add(jla2);jp1.add(jpwf);jp1.add(jla3);
		jp1.add(jcb1);jp1.add(jcb2);jp1.add(jla4);
		
		//把JPanel添加到选项卡窗格
		jtp=new JTabbedPane();
		jtp.add("QQ号码",jp1);
		jtp.add("手机号码",jp3);
		jtp.add("邮箱登录",jp4);
		this.add(jtp);
		
		jp2.add(jb1);jp2.add(jb2);
		this.add(jp2,BorderLayout.SOUTH);
		
		//设置窗体属性
		this.setSize(400, 300);
		this.setLocation(700, 400);
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		this.setTitle("QICQ");
		
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
	}
	public static void main(String[] args) {
		Login lg=new Login();
	}

}
