/*
 * ���ܣ������˵�JComboBox �б��JList ��������JScrollPane
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class BoxListScroll extends JFrame{
	//����ΪJPanel��2����JComboBox��1����JList��1��,JLabel��2��,�Լ�һ����������
	JPanel jp1,jp2;
	JComboBox jcb;//ע����һ�������˵���һ�����
	JList jli;//ע����һ���б����һ�����
	JLabel []jla=new JLabel[2];
	JScrollPane jsp;
	
	BoxListScroll()
	{
		//��ʼ�����
		jp1=new JPanel();jp2=new JPanel();
		jla[0]=new JLabel("���ļ�����");
		jla[1]=new JLabel("����Ҫȥ�����εص���");
		
		String[] aa={"����","�Ϻ�","���","����"};//���ַ������ʼ��JComboBox
		jcb=new JComboBox(aa);	
		
		String[] dd={"����","��������","����","ʲôʲô"};//���ַ������ʼ��JList
		jli=new JList(dd);//JList���߱����칦��
		jli.setVisibleRowCount(3);
		jsp=new JScrollPane(jli);//���б��JList��ӵ����������ٰѹ���������뵽JPanel
		
		//������
		jp1.add(jla[0]);jp1.add(jcb);
		jp2.add(jla[1]);jp2.add(jsp);//ע������ӹ�������JPanel
		this.add(jp1);
		this.add(jp2);
		
		//���ô�������
		this.setLayout(new GridLayout(2,1));
		this.setSize(300,200);
		this.setLocation(700,300);
		this.setTitle("С����");
		
		//���ô�����ʾ���������죬�ر�
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
	}
	
	public static void main(String[] args) {

		BoxListScroll bls=new BoxListScroll();
	}

}
