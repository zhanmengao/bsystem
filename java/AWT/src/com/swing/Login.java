/*
 * ����:ҳǩ��� JTabbedPane:����ǩ����ť��ӵ�ҳǩ��
 */
package com.swing;
import javax.swing.*;
import java.awt.*;
public class Login extends JFrame{
	//����Ϊ��1��JTabbedPane,6��JLabel��1��JTextField,1��JPasswordField,2��JCheckBox,3��JButton
	//�ȶ��山��,������һ��JLabel
	JLabel jlab1=null;
	
	//�����ϲ�����һ��JPanel�Ϸ���2����ť
	JPanel jp2;
	JButton jb1,jb2;
	
	//�����в�����1��JTabbedPane��5��JLabel,2��JCheckBox��1��JTextField,1��JPasswordField,1��JButton
	JPanel jp1,jp3,jp4;
	JTabbedPane jtp;
	JLabel jla1,jla2,jla3,jla4;//QQ����,QQ���룬�һ����룬�������뱣��
	JCheckBox jcb1,jcb2;//��ס���룬�Զ���¼
	JTextField jtf;
	JPasswordField jpwf;
	JButton jb3;//ע���˺�
	
	Login()
	{
		//����
		jlab1=new JLabel(new ImageIcon("images/CAT.jpg"));
		
		//�ϲ�
		jp2=new JPanel();
		jb1=new JButton("��¼");
		jb2=new JButton("ȡ��");
		
		//�в�
		jp1=new JPanel();
		jp3=new JPanel();
		jp4=new JPanel();
		jp1.setLayout(new GridLayout(3,3));
		//jtp=new JTabbedPane();
		jla1=new JLabel("QQ����",JLabel.CENTER);
		jla1.setFont(new Font("����",Font.PLAIN,16));//����������ʽ
		jtf=new JTextField(10);
		jb3=new JButton("ע���˺�");jb3.setForeground(Color.blue);
		
		jla2=new JLabel("QQ����",JLabel.CENTER);
		jla2.setFont(new Font("����",Font.PLAIN,16));//����������ʽ
		jpwf=new JPasswordField(10);
		jla3=new JLabel("�һ�����",JLabel.CENTER);jla3.setForeground(Color.pink);//����������ɫ
		jla3.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));//��������Ƶ����������
		
		jcb1=new JCheckBox("��ס����");
		jcb2=new JCheckBox("�Զ���¼");
		jla4=new JLabel("�������뱣��",JLabel.CENTER);jla4.setForeground(Color.red);//����������ɫ
		jla4.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));//��������Ƶ����������		
		
		//���ò���
		this.setLayout(new BorderLayout());
		
		//������
		this.add(jlab1,BorderLayout.NORTH);
		
	    //jp1.add(jtp);
		//jp1.setBackground(Color.red);
		jp1.add(jla1);jp1.add(jtf);jp1.add(jb3);
		jp1.add(jla2);jp1.add(jpwf);jp1.add(jla3);
		jp1.add(jcb1);jp1.add(jcb2);jp1.add(jla4);
		
		//��JPanel��ӵ�ѡ�����
		jtp=new JTabbedPane();
		jtp.add("QQ����",jp1);
		jtp.add("�ֻ�����",jp3);
		jtp.add("�����¼",jp4);
		this.add(jtp);
		
		jp2.add(jb1);jp2.add(jb2);
		this.add(jp2,BorderLayout.SOUTH);
		
		//���ô�������
		this.setSize(400, 300);
		this.setLocation(700, 400);
		this.setIconImage(new ImageIcon("images/Dogs.jpg").getImage());
		this.setTitle("QICQ");
		
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
	}
	public static void main(String[] args) {
		Login lg=new Login();
	}

}
