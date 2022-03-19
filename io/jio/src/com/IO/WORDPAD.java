/*
 * 功能：记事本的打开和另存为
 */
package com.IO;
import java.awt.*;
import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;

import java.awt.event.*;
import java.io.*;


public class WORDPAD extends JFrame implements ActionListener{
	JTextArea jta=null;
	JScrollPane jsp=null;
	JMenuBar jmb=null;
	JMenu []jm1=new JMenu[1];
	JMenuItem []jmi1=new JMenuItem[2];
	
	public WORDPAD()
	{
		//创建多行文本框
		jta=new JTextArea();
		jsp=new JScrollPane(jta);
		
		
		//创建树干
		jmb=new JMenuBar();
		//创建树枝
		jm1[0]=new JMenu("文件(F)");
		jm1[0].setMnemonic('F');
		//创建树叶
		jmi1[0]=new JMenuItem("打开",new ImageIcon("images/Dogs.jpg"));
		jmi1[0].addActionListener(this);
		jmi1[0].setActionCommand("open");
		
		jmi1[1]=new JMenuItem("另存为");
		jmi1[1].addActionListener(this);
		jmi1[1].setActionCommand("othersave");
		//连接树干树枝树叶
		jmb.add(jm1[0]);
		for(int i=0;i<jmi1.length;i++)
		{
		    jm1[0].add(jmi1[i]);
		}
		
		this.setJMenuBar(jmb);
		this.add(jsp);
		
		this.setSize(900,500);
		this.setLocation(700,300);
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	}
	

	public static void main(String[] args) {
		WORDPAD wp=new WORDPAD();

	}


	@Override
	public void actionPerformed(ActionEvent e) {
		//处理监听
		if(e.getActionCommand().equals("open"))
		{
			//如果点了打开
			System.out.println("打开");
			//JFileChooser：文件选择窗体
			JFileChooser jfc=new JFileChooser();
			jfc.setVerifyInputWhenFocusTarget(true);//设置显示
			jfc.setDialogTitle("打开文件");//设置窗体标题
			jfc.showOpenDialog(null);
			
			//获取用户打开的地址
			String address=jfc.getSelectedFile().getAbsolutePath();
			//用该地址读取文件
			FileReader f=null;
			BufferedReader br=null;
			try {
				f = new FileReader(address);
				br=new BufferedReader(f);
				//读取字符串
				String s="";
				String all="";
				//把每一行放入到s
				while((s=br.readLine())!=null)
				{
					//把每个s添加到all
					all+=s+"\r\n";
				}
				//把all添加到jta
				jta.setText(all);
			} catch (Exception e1) {
				e1.printStackTrace();
			}finally{
				try {
					br.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				try {
					f.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			
			
			
		}else if(e.getActionCommand().equals("othersave"))
		{
			//如果点了另存为
			System.out.println("另存为");
			//JFileChooser：文件选择窗体
			JFileChooser jfc=new JFileChooser();
			jfc.setVerifyInputWhenFocusTarget(true);//设置显示
			jfc.setDialogTitle("另存为");//设置窗体标题
			jfc.showSaveDialog(null);//设置默认方式打开
			
			//获取用户选择的地址
			String address=jfc.getSelectedFile().getAbsolutePath();
			//用该地址作输出流的地址
			FileWriter fw=null;
			BufferedWriter bw=null;
			try {
				fw=new FileWriter(address);
				bw=new BufferedWriter(fw);
				
				String s=this.jta.getText();//获取多行文本框内容
				char c[]=new char[1024];
				//一口气写到硬盘
				bw.write(s);
				
			} catch (IOException e1) {
				System.out.println("写入异常");
				e1.printStackTrace();
			}finally{
				try {
					bw.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				try {
					System.out.println("关闭FileWriter");
					fw.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			
		}
		
	}

}
