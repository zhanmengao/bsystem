/*
 * ���ܣ���ѡ��JCheckBox,��ѡ��JRadioButton,��ѡ����ButtonGroup
 */
package com.swing;
import java.awt.*;
import javax.swing.*;;

public class Box extends JFrame{
	//����Ϊ��Ҫ3��JPanel��3��JCheckBox��2��JRadioButton��2��JLael��2��JButton
	JPanel jp[]=new JPanel[3];
	JCheckBox jcb[]=new JCheckBox[3];
	JRadioButton jrb[]=new JRadioButton[2];
	JLabel []jla=new JLabel[2];
	JButton []jbu=new JButton[2];
	ButtonGroup bg;
	
	Box()
	{
		//��ѡ������Ϊ�����������򣬵�ѡ������Ϊ��/Ů
		jp[0]=new JPanel();jp[1]=new JPanel();jp[2]=new JPanel();
		
		jla[0]=new JLabel("��ϲ�����˶���");
		jcb[0]=new JCheckBox("����");jcb[1]=new JCheckBox("����");jcb[2]=new JCheckBox("����");
		
		jla[1]=new JLabel("����Ա���");
		jrb[0]=new JRadioButton("��");jrb[1]=new JRadioButton("Ů");
		bg=new ButtonGroup();bg.add(jrb[0]);bg.add(jrb[1]);//��ӵ�ѡ�򵽵�ѡ����
		
		
		
		jbu[0]=new JButton("ע���û�");jbu[1]=new JButton("ȡ��ע��");
		
		//��������
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
		
		
		//���ô�������
		this.setLayout(new GridLayout(3,1));
		this.setSize(400,200);
		this.setLocation(700, 360);
		this.setTitle("�û�ע�����");
		
		//���ô�����ʾ����ֹ���졢�ر�
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		Box bo=new Box();
	}

}
