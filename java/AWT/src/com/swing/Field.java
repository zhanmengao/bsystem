/*
 * ���ܣ�JTextField�ı���JPasswordField�����JLabel��ǩ
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class Field extends JFrame{
	//�������������Ϊ��3��JPanel��һ��JTextField��JPasswordField������JLabel������JButton
	JPanel jp[]=new JPanel[3];
	JTextField jtext1;
	JPasswordField jpass1;
	JLabel []jla=new JLabel[2];
	JButton []jbu=new JButton[2];
	
	Field()
	{
		//��ʼ�����
		jp[0]=new JPanel();jp[1]=new JPanel();jp[2]=new JPanel();
		jtext1=new JTextField(10);
		jpass1=new JPasswordField(10);
		jla[0]=new JLabel("�û���");
		jla[1]=new JLabel("��    ��");
		jbu[0]=new JButton("ȷ��");jbu[1]=new JButton("ȡ��");
		
		//������
		jp[0].add(jla[0]);jp[0].add(jtext1);
		jp[1].add(jla[1]);jp[1].add(jpass1);
		jp[2].add(jbu[0]);jp[2].add(jbu[1]);
		for(int i=0;i<jp.length;i++)
		{
			this.add(jp[i]);
		}
		
		//���ò��֡���������
		this.setLayout(new GridLayout(3,1));
		this.setSize(300,200);
		this.setLocation(700, 350);
		this.setTitle("�������ϵͳ");
		
		//������ʾ���رա���������
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}

	public static void main(String[] args) {
		Field fi=new Field();
	}

}
