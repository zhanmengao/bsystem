/*
 * ���ܣ���һ�����±�����
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class Mean extends JFrame{
	JScrollPane jsp=null;
	JTextArea jta=null;
	//����Ϊ������1���˵���
	JMenuBar jmb=null;
	//һ���˵�������5���˵�
	JMenu jm1[]=new JMenu[5];
	//����1��1������7����ѡ��
	JMenuItem []jmi1=new JMenuItem[7];
	
	//����1,1���½����棬���У��ļ������̣�.����дһ�������˵�
	JMenuItem wenben=null;
	JMenuItem xiangmu=null;
	
	//������
	JToolBar jtb=null;
	JButton jb1[]=new JButton[6];
	
	//����
	public Mean()
	{
		jta=new JTextArea();
		jsp=new JScrollPane(jta);
		jmb=new JMenuBar();
		
		jm1[0]=new JMenu("�ļ�(F)");
		jm1[0].setMnemonic('F');
		jm1[1]=new JMenu("�༭(E)");
		jm1[1].setMnemonic('E');
		jm1[2]=new JMenu("��ʽ(O)");
		jm1[2].setMnemonic('O');
		jm1[3]=new JMenu("�鿴(V)");
		jm1[3].setMnemonic('V');
		jm1[4]=new JMenu("����(H)");
		jm1[4].setMnemonic('H');
		
		for(int i=0;i<jm1.length;i++)
		{
			jmb.add(jm1[i]);
		}
		//��ʼ����Ҷ
		jmi1[0]=new JMenu("�½�");
		jmi1[1]=new JMenuItem("��");
		jmi1[2]=new JMenuItem("����");
		jmi1[3]=new JMenuItem("���Ϊ");
		jmi1[4]=new JMenuItem("ҳ������");
		jmi1[5]=new JMenuItem("��ӡ");
		jmi1[6]=new JMenuItem("�˳�");
		
		//�ٸ���Ҷ���½��������������Ҷ���ı�����Ŀ��
		wenben=new JMenuItem("�ı�");
		xiangmu=new JMenuItem("��Ŀ");
		jmi1[0].add(wenben);
		jmi1[0].add(xiangmu);
		//����Ҷ��ӵ���������֦
		for(int i=0;i<jmi1.length;i++)
		{
			jm1[0].add(jmi1[i]);
			if(i==3 || i==5)
			{
				//��ӷָ���
				jm1[0].addSeparator();
			}
		}

		
		jtb=new JToolBar();
		jb1[0]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[0].setToolTipText("�½�");//������ʾ��Ϣ
		jb1[1]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[1].setToolTipText("����");//������ʾ��Ϣ
		jb1[2]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[2].setToolTipText("��");//������ʾ��Ϣ
		jb1[3]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[4]=new JButton(new ImageIcon("image/Dogs.jpg"));
		jb1[5]=new JButton(new ImageIcon("image/Dogs.jpg"));
		for(int i=0;i<jb1.length;i++)
		{
			jtb.add(jb1[i]);
		}
		
		//���˵�����ӵ�������(setJMenuBar)
		this.setJMenuBar(jmb);		
		//����������ӵ�������
		this.add(jtb,BorderLayout.NORTH);
		
		JScrollPane jsp=new JScrollPane(jta);
		jsp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
		this.add(jsp);
		
		this.setLocation(500, 300);
		this.setSize(900,600);
		this.setTitle("���±�");
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}
	

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Mean m=new Mean();
	}

}
