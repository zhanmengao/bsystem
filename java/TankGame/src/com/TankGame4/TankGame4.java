/*
 * ���ܣ����̹�˴�ս3.1(���������֣�
 * �Ż���̹���ṩ����෢��10���ӵ��Ĺ���
 */
package com.TankGame4;
import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
import java.util.*;


public class TankGame4 extends JFrame implements KeyListener,ActionListener{
	//��ʼ��ǰ�ȹ���ö�������-���
	MyPanel5 mp2=null;
	int xlength=630;
	public int getXlength() {
		return xlength;
	}
	public void setXlength(int xlength) {
		this.xlength = xlength;
	}
	public int getYlength() {
		return ylength;
	}
	public void setYlength(int ylength) {
		this.ylength = ylength;
	}
	int ylength=600;
	
	GamePrepare gp=null;
	
	//����˵�
	JMenuBar jmb=null;
	JMenu[] jm=new JMenu[5];
	JMenuItem []jmi=new JMenuItem[4];
	
	TankGame4()
	{
		//��ʼ�����ɡ���֦
		jmb=new JMenuBar();
		jm[0]=new JMenu("��Ϸ");
		//��ʼ����Ҷ
		jmi[0]=new JMenuItem("����Ϸ");
		jmi[1]=new JMenuItem("����");
		jmi[2]=new JMenuItem("��ȡ");
		jmi[3]=new JMenuItem("�˳�");
		jmb.add(jm[0]);
		for(int i=0;i<jmi.length;i++)
		{
			jm[0].add(jmi[i]);
			jmi[i].addActionListener(this);
		}
		this.setJMenuBar(jmb);
		//������Ϸ ע�������ί��JFrame�����
		jmi[0].setActionCommand("StartGame");
		jmi[1].setActionCommand("save");
		jmi[2].setActionCommand("read");
		jmi[3].setActionCommand("exit");
		
		//�ѿ�ʼ������ӵ�JFrame
		gp=new GamePrepare();
		this.add(gp);
		//���� gp�̣߳���˸��
		Thread t1=new Thread(gp);
		t1.start();
		//ע������������Լ���������
		this.addKeyListener(this);
		
		this.setSize(xlength,ylength);
		this.setLocation(700, 300);
		this.setTitle("̹�˴�ս4.0");
		
		this.setVisible(true);
		this.setResizable(false);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
	public static void main(String[] args) {
		TankGame4 tg2=new TankGame4();
	}
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void keyPressed(KeyEvent e) {
		//���¿ո񣺿�ʼ/��ͣ
		if(e.getKeyCode()==KeyEvent.VK_SPACE)
		{
				MyPanel5.isPaused=!MyPanel5.isPaused;
		}
		
		
	}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		//������˿�ʼ��Ϸ�˵�
		String s=e.getActionCommand();
		if(s.equals("StartGame"))
		{
			AePlayWave a=new AePlayWave("C:\\CloudMusic\\Ⱥ�� - FC��̹�˴�ս��սǰBGM.mp3");
			a.start();
			//ɾ�������
			this.remove(gp);
			mp2=new MyPanel5(false);
			this.add(mp2);
			//�����������ʾ
			this.setVisible(true);
			this.addKeyListener(mp2);
	        //����mp�߳�  
	        Thread t=new Thread(mp2);  
	        t.start(); 
		}else if(s.equals("save"))
		{
			System.out.println("����");
			//�ڵ�������ʱ�򣬰�����������¼�ࡣ����ȥд
			new Record().setV(mp2.emy);
			//���浱ǰ�ֵܷ��ļ�	
			Record.Write();
			
			
			
		}else if(s.equals("read"))
		{
			
			System.out.println("��ȡ");
			//ɾ�������
			this.remove(gp);
			mp2=new MyPanel5(true);
			this.add(mp2);
			//�����������ʾ
			this.setVisible(true);
			this.addKeyListener(mp2);
	        //����mp�߳�  
	        Thread t=new Thread(mp2);  
	        t.start();
		}
		else if(s.equals("exit"))
		{
			System.out.println("�˳�");
			System.exit(0);
		}
	}
	

}
//�ҵĻ��壨׼�����棩
class GamePrepare extends JPanel implements Runnable{
	static int time=0;
	int i=1;
	public void paint(Graphics g)
	{
		super.paint(g);
		//��һ�����Σ����滭�ַ�������׼������
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 500, 400);
		//�����ַ���
		g.setColor(Color.RED);
		Font myfont=new Font("������κ",Font.BOLD,30);
		g.setFont(myfont);
		 //������˸Ч����ÿ0.5��ı�һ��timeֵ��
		if(time%2==0)
		{
			g.drawString("��"+(i)+"��", 200, 200);
		}
	}

	@Override
	public void run() {
		while(true)
		{
		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		//ÿ0.5���ػ棬ǰ0.5����ʾString����0.5�벻����ʾ
		repaint();
		time++;
		}
	}
}
//�ҵĻ��壨��Ϸ��壩
class MyPanel5 extends JPanel implements KeyListener,Runnable{
	//��ͣ�Ĳ���
	static boolean isPaused=false;
	//������˵�̹����
	Vector<Enemy> emy=new Vector<Enemy>();//ָ��ֻ�ܷ��غͽ���Enemy����
	static int tote=5;//�������5̨����̹��
	
	//�ڻ����϶���һ���ҵ�̹��
	Hero4 myhero=null;
	public MyPanel5(boolean read)
	{
		//���Ƿ��ȡ
		if(read==false)
		{
			Record.get_total();
			//�����ҵ�̹��
			myhero=new Hero4(50,345,1,1);
			//�������5��̹��
			for(int i=0;i<tote;i++)
			{
				//����3��̹�˶��󣬸�����ʼ����
				Enemy enemytank=enemytank=new Enemy(60*(i+1),10,2,2);
				emy.add(enemytank);
				//�������enemytank�����������Լ�(�����ʱ�����Enemy������)
				enemytank.setEenemy(emy);
				//��ʼ����ʱ�������л��߳�
				Thread t=new Thread(enemytank);
				t.start();
				}
			}
		else if(read==true)
		{
			//��MyPanel֪����¼��������
			Vector<Node> node=new Vector<Node>();
			node=Record.getNode();
			//�����ҵ�̹��
			myhero=new Hero4(50,345,1,1);
			//�������5��̹��
			for(int i=0;i<node.size();i++)
			{
				Node no=node.get(i);
				//����3��̹�˶��󣬸�����ʼ����
				Enemy enemytank=enemytank=new Enemy(no.x,no.y,no.direct,2);
				emy.add(enemytank);
				//�������enemytank�����������Լ�(�����ʱ�����Enemy������)
				enemytank.setEenemy(emy);
				//��ʼ����ʱ�������л��߳�
				Thread t=new Thread(enemytank);
				t.start();
				}
		}
	}
	//׼��һ������ʾ��Ϣ�ĺ���
	public void ShowRecord(Graphics g)
	{
		//������¼̹��
		//�����ǵ���
		this.drawtank(80, 450, g, 1, 2);
		g.setColor(Color.black);
		g.setFont(new Font("΢���ź�",Font.BOLD,20));
		g.drawString(""+Record.getResidue(), 100, 450);
			
		//���Լ�
		this.drawtank(200, 450, g, 1, 1);
		g.setColor(Color.black);
		g.drawString(Record.getMylife()+"", 220, 450);
		
		//�����ұ߼Ʒ�
		g.drawString("�����ܷ�",510, 50);
		g.drawString(Record.getTotal()+"", 510, 100);
		g.drawString("��ɱ", 510, 150);
		this.drawtank(520, 180, g, 1, 2);
		g.setColor(Color.black);
		g.drawString("��"+Record.getKill(), 535, 186);

	}
	//����paint(g),ÿ100����ˢ��һ��
	public void paint(Graphics g)
	{
		super.paint(g);
		//�ָ��ܷ�
		g.setColor(Color.DARK_GRAY);
		g.fillRect(0, 0, 500,400);	
		this.ShowRecord(g);
		//����ͷ����Ǳ���
		if(myhero.isLive==true)
		{
			this.drawtank(myhero.getX(), myhero.getY(), g, myhero.getDirect(), myhero.getColor());
		}
		//����̹�˼���
		for(int i=0;i<emy.size();i++)
		{
			Enemy enemytank=emy.get(i);
			//��Vector.get(index)����ȡemy��Ķ�����Ϣ		
			if(emy.get(i).isLive==true)
			{
				this.drawtank(enemytank.getX(),enemytank.getY(),g,enemytank.getDirect(),enemytank.getColor());
			}
			//�����ӵ�����
			for(int j=0;j<enemytank.bu.size();j++)
			{
				//�Ӽ�����ȡ���ӵ�
				Bulle4 s=enemytank.bu.get(j);
				//���˷����ӵ�
			    if(s.isLive==true)
			    {
			    	g.setColor(Color.yellow);
			    	//���ӵ�(�����ڱ䣬������ṩ��
			    	g.fillOval(s.getX(), s.getY(), 2, 2);
			    }
		     }
		}
		for(int i=0;i<myhero.bu.size();i++)
		{
			//ȡ���ӵ�
			Bulle4 bl=myhero.bu.get(i);
			//�ҷ����ӵ�
			if(bl!=null && bl.isLive==true)
			{
				g.setColor(Color.white);
				//���ӵ�(�����ڱ䣬������ṩ��
				g.fillOval(bl.getX(), bl.getY(), 2, 2);
			}
		}
	}
	//�������̹���ܶ�û�н��������ٹ���
	public void creatEnemy()
	{
		//�����������0
		while((Record.getResidue()-emy.size())>0 && tote-emy.size()>0)
		{
			boolean b=true;
			int ranx=30+(int)(Math.random()*(150-30));
			int rany=30+(int)(Math.random()*(150-30));
			//�����������̹���ص��������һ���ص������ص���
			for(int i=0;i<emy.size();i++)
			{
				if(!(ranx-emy.get(i).x>30) && !(emy.get(i).x-ranx>30)
						&& !(rany-emy.get(i).y>30) && !(emy.get(i).y-rany>30))
				{
					b=false;
				}
			}
			//��λ�ÿ��Դ���
			if(b==true)
				{
				//�����µ�̹�ˣ���������Ļ����-��Ļ���
				for(int j=0;j<tote-emy.size();j++)
				{
					Enemy enemytank=new Enemy(ranx,rany,2,2);
					emy.add(enemytank);
					//�������enemytank�����������Լ�(�����ʱ�����Enemy������)
					enemytank.setEenemy(emy);
					//��ʼ����ʱ�������л��߳�
					Thread t=new Thread(enemytank);
					t.start();
				}
			}
			}
		}
	
	//�������ӵ����ҵ�̹��������
	public void EnymybuAndMe()
	{
		//ȡ��ÿһ������
		for(int i=0;i<emy.size();i++)
		{
			Enemy em=emy.get(i);
			//ȡ���õ��˵�ÿһ���ӵ�
			for(int j=0;j<em.bu.size();j++)
			{
				Bulle4 s=em.bu.get(j);
				
				//����ƥ��
				if(myhero.isLive==true && em.bu.get(j).isLive==true)
				{
					this.dead(s, myhero);
				}
				//������������Ƴ������ӵ�
				if(em.bu.get(j).isLive==false)
				{
					em.bu.remove(em.bu.get(j));
				}
				if(myhero.isLive==false)
				{
					Record.killme();
					//���������
					if(Record.getMylife()>0)
					{
						//�����ҵ�̹��
						myhero=new Hero4(50,345,1,1);
					}
				}
			}
		}
	}
	//����ҵ��ӵ������̹��������
	public void MybuAndEnemy()
	{
		for(int i=0;i<emy.size();i++)
		{
			Enemy em=emy.get(i);
			for(int j=0;j<myhero.bu.size();j++)
			{
				Bulle4 s=myhero.bu.get(j);
				//��������ж�λ��
				if(s.isLive==true && em.isLive==true)
				{
					this.dead(s, em);
				}
				//������������Ƴ�����
				if(em.isLive==false)
				{
					emy.remove(em);
					Record.killenemy();
				}
				//������������Ƴ��ҵ��ӵ�
				if(s.isLive==false)
				{
					myhero.bu.remove(s);
				}
			}
		}
	}
	
	//�ж�̹�ˣ��ӵ��Ĵ��ṩһ���ӵ���һ̨̹�ˣ�
	public void dead(Bulle4 bu,Tank4 et)
	{
		//1���ӵ�����ʱ����
		if(bu.getX()>499 && bu.getX()<1 ||
				bu.getY()<1 || bu.getY()>399 )
		{
			bu.isLive=false;
		}
		//2������̹�����ӵ���ײ����(�ж�ÿ��ͼ�����ӵ��Ƿ�Ӵ���
		//bu(x1,y1,w1,h1),et(x2,y2,w2,h2)
		switch(et.direct)
		{
		case 1:
			if((bu.x-et.x+11<5 && et.x-11-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x-6<5 && et.x+6-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x+6<12 && et.x-6-bu.x<12 && bu.y-et.y+5<12 && et.y-5-bu.y<2))
			{
				//����
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 2:
			if((bu.x-et.x+11<5 && et.x-11-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x-6<5 && et.x+6-bu.x<2 && bu.y-et.y+12<20 && et.y-12-bu.y<2)||
					(bu.x-et.x+6<12 && et.x-6-bu.x<12 && bu.y-et.y+10<12 && et.y-10-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 3:
			if((bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y+11<5 && et.y-11-bu.y<2)||
					(bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y-6<5 && et.y+6-bu.y<2)||
					(bu.x-et.x-3<12 && et.x+3-bu.x<12 && bu.y-et.y+6<12 && et.y-6-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;
			
		case 4:
			if((bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y+11<5 && et.y-11-bu.y<2)||
					(bu.x-et.x+5<20 && et.x-5-bu.x<2 && bu.y-et.y-6<5 && et.y+6-bu.y<2)||
					(bu.x-et.x+5<12 && et.x+-5-bu.x<12 && bu.y-et.y+6<12 && et.y-6-bu.y<2))
			{
				et.isLive=false;
				bu.isLive=false;
			}
			break;	
		}
	}
	//дһ����̹�˵ĺ�������Ҫ��̹�˵�ǰλ�ã�̹�����߷���̹������
	public void drawtank(int x,int y,Graphics g,int direct,int type)
	{
		//����̹�������ж���ɫ
		switch(type)
		{
		case 1://�ҷ�̹��
			g.setColor(Color.CYAN);
			break;
		case 2://�з�5��̹��
			g.setColor(Color.yellow);
			break;
		} 
		//����
		switch(direct)
		{
		//�����1�����ϻ�
				case 1:
					//1��������߾���
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2�������ұ߾���
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3�������м����
					g.fill3DRect(x-6, y-5, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-5, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x, y-12);
					break;
				//��2�����»�
				case 2:
					//1��������߾���
					g.fill3DRect(x-11, y-12, 5, 20,true);
					//2�������ұ߾���
					g.fill3DRect(x+6, y-12, 5, 20, true);
					//3�������м����
					g.fill3DRect(x-6, y-10, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-5, y-7,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x, y+8);
					break;
				//3��������
				case 3:
					//1��������߾���
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2�������ұ߾���
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3�������м����
					g.fill3DRect(x+3, y-6, 12,12, true);
					//4�������м�Բ��
					//g.fillOval(x-4, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x+7, y, x-8, y);
					break;
				case 4:
					//1��������߾���
					g.fill3DRect(x-5, y-11, 20, 5,true);
					//2�������ұ߾���
					g.fill3DRect(x-5, y+6, 20, 5, true);
					//3�������м����
					g.fill3DRect(x-5, y-6, 12,12, true);
					//4�������м�Բ��
					g.fillOval(x-4, y-4,8,8);
					//5��������Ͳ
					g.drawLine(x, y, x+15, y);
					break;
		}

	}
	@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	//������̰����������,w��s��a��d��
	public void keyPressed(KeyEvent e) {
		int keycode=e.getKeyCode();
		//�����������ͣ����������Ч
		if(isPaused==false)
		{
		switch(keycode)
		{
		case KeyEvent.VK_W:
			this.myhero.setDirect(1);
			this.myhero.move(1);
			//���������ߵķ���
			break;
		case KeyEvent.VK_S:
			this.myhero.setDirect(2);
			this.myhero.move(2);
			//���������ߵķ���
			break;
		case KeyEvent.VK_A:
			this.myhero.setDirect(3);
			this.myhero.move(3);
			//���������ߵķ���
			break;
		case KeyEvent.VK_D:
			this.myhero.setDirect(4);
			this.myhero.move(4);
			//���������ߵķ��� 
			break;
		}
	//���������Ƿ���J��
	if(e.getKeyCode()==KeyEvent.VK_J)
	{
		//���ÿ�������
		this.myhero.biubiu();
		this.myhero.s.dxdy=20;
	}
	this.repaint();	
	}
}
	@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
	@Override
	//�����ҵĻ������ҳ��
	public void run() 
	{
		int time=0;
		while(true)
		{
			if(isPaused==false)
			{
			//ÿ10�������ӵ���̹��
			this.MybuAndEnemy();
			this.EnymybuAndMe();
			if(time%200==0)
			{
				this.creatEnemy();
			}
			time++;
			}
			//ÿ10���ػ�
			this.repaint();
			try {
				Thread.sleep(5);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
