/*
 * ���ܣ�JTextArea�����ı���
 */
package com.swing;
import java.awt.*;
import javax.swing.*;
public class TextArea extends JFrame{
	//����Ϊ������1��JTextArea��1��JComboBox,1��JTextField��1��JButton
	JPanel jpanel1=null;
	JTextArea jtextarea1=null;JScrollPane jscrollpane1=null;
	JComboBox jcombobox1=null;
	JTextField jtextfield1=null;
	JButton jbutton1=null;
	
	TextArea()
	{
		//��ʼ�� �����˵�����Ϊ�����˵���������ť����Ϊ����
		jpanel1=new JPanel();
		jtextarea1=new JTextArea();
		jscrollpane1=new JScrollPane(jtextarea1);//�������ı�����ӵ���������ʵ�ֿ������»���
		
		String friends[]={"����","����","����","�װ�"};
		jcombobox1=new JComboBox(friends);
		jtextfield1=new JTextField(10);
		jbutton1=new JButton("����");
		
		//���ò���
		this.setLayout(new BorderLayout());
		
		//������
		jpanel1.add(jcombobox1);
		jpanel1.add(jtextfield1);
		jpanel1.add(jbutton1);
		this.add(jscrollpane1);
		this.add(jpanel1,BorderLayout.SOUTH);

		
		//���ô�������
		this.setSize(500, 300);
		this.setLocation(500, 500);
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		this.setTitle("������");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {

		TextArea ta=new TextArea();
	}

}
