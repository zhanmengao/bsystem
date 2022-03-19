/*
 * ���ܣ����±��Ĵ򿪺����Ϊ
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
		//���������ı���
		jta=new JTextArea();
		jsp=new JScrollPane(jta);
		
		
		//��������
		jmb=new JMenuBar();
		//������֦
		jm1[0]=new JMenu("�ļ�(F)");
		jm1[0].setMnemonic('F');
		//������Ҷ
		jmi1[0]=new JMenuItem("��",new ImageIcon("images/Dogs.jpg"));
		jmi1[0].addActionListener(this);
		jmi1[0].setActionCommand("open");
		
		jmi1[1]=new JMenuItem("���Ϊ");
		jmi1[1].addActionListener(this);
		jmi1[1].setActionCommand("othersave");
		//����������֦��Ҷ
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
		//�������
		if(e.getActionCommand().equals("open"))
		{
			//������˴�
			System.out.println("��");
			//JFileChooser���ļ�ѡ����
			JFileChooser jfc=new JFileChooser();
			jfc.setVerifyInputWhenFocusTarget(true);//������ʾ
			jfc.setDialogTitle("���ļ�");//���ô������
			jfc.showOpenDialog(null);
			
			//��ȡ�û��򿪵ĵ�ַ
			String address=jfc.getSelectedFile().getAbsolutePath();
			//�øõ�ַ��ȡ�ļ�
			FileReader f=null;
			BufferedReader br=null;
			try {
				f = new FileReader(address);
				br=new BufferedReader(f);
				//��ȡ�ַ���
				String s="";
				String all="";
				//��ÿһ�з��뵽s
				while((s=br.readLine())!=null)
				{
					//��ÿ��s��ӵ�all
					all+=s+"\r\n";
				}
				//��all��ӵ�jta
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
			//����������Ϊ
			System.out.println("���Ϊ");
			//JFileChooser���ļ�ѡ����
			JFileChooser jfc=new JFileChooser();
			jfc.setVerifyInputWhenFocusTarget(true);//������ʾ
			jfc.setDialogTitle("���Ϊ");//���ô������
			jfc.showSaveDialog(null);//����Ĭ�Ϸ�ʽ��
			
			//��ȡ�û�ѡ��ĵ�ַ
			String address=jfc.getSelectedFile().getAbsolutePath();
			//�øõ�ַ��������ĵ�ַ
			FileWriter fw=null;
			BufferedWriter bw=null;
			try {
				fw=new FileWriter(address);
				bw=new BufferedWriter(fw);
				
				String s=this.jta.getText();//��ȡ�����ı�������
				char c[]=new char[1024];
				//һ����д��Ӳ��
				bw.write(s);
				
			} catch (IOException e1) {
				System.out.println("д���쳣");
				e1.printStackTrace();
			}finally{
				try {
					bw.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				try {
					System.out.println("�ر�FileWriter");
					fw.close();
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			
		}
		
	}

}
