/*
 * ͼ��鿴��
 */
package com.swing;
import java.awt.*;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.*;
//���������ӦΪ6*2�����񲼾֣��ұ߿ղ��֡�����1*2�����񲼾�

public class Book extends JFrame implements ActionListener{
	//���6����ǩ��6�������ı���
	JLabel []jla=new JLabel[2];//�����嶥���������
	JPanel jp1=null;
	JLabel jl1[]=new JLabel[6];
	JTextField jtf1[]=new JTextField[7];
	
	//�ұߣ�7��JButton,һ��JLabel����ʾͼƬ��
	JPanel jp2=null;
	JPanel jp3=null;
	JButton jb1[]=new JButton[6];
	JLabel jb2=null;
	
	
	Book()
	{
		this.setLayout(new GridLayout(1,2,20,20));
		//�ȹ������
		jp1=new JPanel();
		jp1.setLayout(new GridLayout(7,2,10,30));
		//�����봰�嶥���������
		jla[0]=new JLabel(" ");jla[1]=new JLabel(" ");
		jp1.add(jla[0]);jp1.add(jla[1]);
		
		jl1[0]=new JLabel("����",JLabel.CENTER);
		jl1[1]=new JLabel("����",JLabel.CENTER);
		jl1[2]=new JLabel("������",JLabel.CENTER);
		jl1[3]=new JLabel("������",JLabel.CENTER);
		jl1[4]=new JLabel("״̬",JLabel.CENTER);
		jl1[5]=new JLabel("���۱���",JLabel.CENTER);
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
		
		//�ٹ����ұߣ�����Ϊ�����񲼾֣���JPanel jp2����������JPanel,mpp��jp3
		jp2=new JPanel();
		jp3=new JPanel();
		jp2.setLayout(new GridLayout(2,1,50,50));
		//�ϲ�����һ��ͼƬ
		MyPanelPicture mpp=new MyPanelPicture();
		jp2.add(mpp);
		
		jp3.setLayout(new GridLayout(3,2,50,50));
		jb1[0]=new JButton("��һ��");
		jb1[1]=new JButton("��һ��");
		jb1[2]=new JButton("���");
		jb1[3]=new JButton("����");
		jb1[4]=new JButton("ɾ��");
		jb1[5]=new JButton("�ر�");
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





