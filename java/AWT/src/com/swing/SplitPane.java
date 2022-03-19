/*
 * ���ܣ���ִ��� JSplitPane
 */
package com.swing;
import java.awt.*;
import javax.swing.*;

public class SplitPane extends JFrame {
	//����Ϊ���������Ҫ1��JLabel��1��JList��1��JSplitPane
	JSplitPane jsplitpane1;
	JLabel jlabel1;
	JList jlist1;

	
	SplitPane()
	{
		//�ȶ���JList������Ϊ����������JLabe������ΪͼƬ����
		String[] ox={"����ʿʽ","����ʽ","�Ϻ��Ƴ�","Ů��ʽ"};
		jlist1=new JList(ox);
		jlabel1=new JLabel(new ImageIcon("images/IMG_1410.JPG"));
		jsplitpane1=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,jlist1,jlabel1);
		jsplitpane1.setOneTouchExpandable(true);//��������
		
		//���ò���
		this.setLayout(new BorderLayout());
		
		//������
		this.add(jsplitpane1);
		
		//���ô������
		this.setSize(900, 1000);
		this.setLocation(600, 300);
		
		//������ʾ �ر� ��������
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
	}

	public static void main(String[] args) {
		SplitPane sp=new SplitPane();

	}

}
