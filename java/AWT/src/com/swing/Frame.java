/*
 * ���ܣ���ϰJFrame,JButton,BorderLayout,FlowLayout,GridLayout��ʹ��
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class Frame extends JFrame{
	//�½����
	int size=5;
	JButton jb[]=new JButton[size];
	
	Frame()
	{
		//��ʼ�����
		jb[0]=new JButton("�������壬һ������");
		jb[1]=new JButton("���������μ���");
		jb[2]=new JButton("��������������и��£�");
		jb[3]=new JButton("��������ɣ�");
		jb[4]=new JButton("���֮����������");
		
		//�߽粼�ֵ����
		this.add(jb[0],BorderLayout.CENTER);
		this.add(jb[1],BorderLayout.NORTH);
		this.add(jb[2],BorderLayout.SOUTH);
		this.add(jb[3],BorderLayout.WEST);
		this.add(jb[4],BorderLayout.EAST);
	
		//������,��ʧ���ֺ����񲼾ֶ�����
		/*
		 for(int i=0;i<5;i++)
		{
			this.add(jb[i]);
		}
		*/
		
		//����JFrame���֡�����
		//this.setLayout(new BorderLayout());
//		this.setLayout(new FlowLayout(FlowLayout.LEFT));
		//this.setLayout(new GridLayout(5,1,10,10));
		this.setSize(500,500);
		this.setLocation(300, 500);
		this.setTitle("������ҫ");
		
		//����JFrame�������졢��ʾ���ر���ʽ
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
	}

	public static void main(String[] args) {
		Frame fr=new Frame();
	}

}
