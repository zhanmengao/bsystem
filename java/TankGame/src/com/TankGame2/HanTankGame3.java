package com.TankGame2;

import javax.swing.*;   

import java.awt.*;  
import java.awt.event.*;  
import java.util.*;  
  
  
//�ҵ�̹�˴�ս��Ϸ3.0�汾  
/** 
 * �ҵ�̹�˿������������ƶ� 
 *  
 * @author Administrator 
 * 
 */  
  
public class HanTankGame3 extends JFrame {  
  
    MyPanel mp = null;  
  
    public static void main(String[] args) {  
        HanTankGame3 mtg = new HanTankGame3();  
    }  
  
    // ���캯��  
    public HanTankGame3() {  
        mp = new MyPanel();  
          
        //����mp�߳�  
        Thread t=new Thread(mp);  
        t.start();        
          
        this.add(mp);  
        // ע�����  
        this.addKeyListener(mp);  
  
        this.setSize(400, 300);  
        this.setVisible(true);  
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }  
  
}  
  
// �ҵ����  
class MyPanel extends JPanel implements KeyListener,Runnable{  
  
    // ����һ���ҵ�̹��  
    Hero hero = null;  
      
    //������˵�̹����  
    Vector<EnemyTank> ets=new Vector<EnemyTank>();  
  
    //�������̹�˵�����  
    int enSize=3;     
      
    // ���캯��  
    public MyPanel() {  
        hero = new Hero(100, 100);        
        //��ʼ�����˵�̹��  
        for(int i=0;i<enSize;i++){  
            //����һ�����˵�̹�˶���  
            EnemyTank et=new EnemyTank((i+1)*50,0);  
            et.setColor(0);  
            et.setDirect(2);  
            //���뵽vector������  
            ets.add(et);  
        }  
    }  
  
    // ��дpaint  
    public void paint(Graphics g) {  
        super.paint(g);  
        // �ѱ���ɫ��ɺ�ɫ  
        g.fillRect(0, 0, 400, 300);  
        g.setColor(Color.CYAN);  
  
        // �����Լ���̹��  
        this.drawTank(hero.getX(), hero.getY(), g, this.hero.direct, 1);  
          
        //�����ӵ�  
        if(hero.s!=null && hero.s.isLive==true){
            g.draw3DRect(hero.s.x,hero.s.y,1,1,false);  
        }  
          
        //�������˵�̹��  
        for(int i=0;i<ets.size();i++){             
            this.drawTank(ets.get(i).getX(), ets.get(i).getY(), g, ets.get(i).getDirect(), 0);  
        }  
  
    }  
  
    // ����̹�˵ĺ���  
    public void drawTank(int x, int y, Graphics g, int direct, int type) {  
        // �ж���ʲô���͵�̹��  
        switch (type) {  
        case 0:  
            g.setColor(Color.CYAN);  
            break;  
        case 1:  
            g.setColor(Color.yellow);  
            break;  
        }  
  
        // �жϷ���  
        switch (direct) {  
        // ����  
        case 0:  
            // �����ҵ�̹��  
            // 1.������ߵľ���  
            g.fill3DRect(x, y, 5, 30, false);  
            // 2.�����ұߵľ���  
            g.fill3DRect(x + 15, y, 5, 30, false);  
            // 3.�����м����  
            g.fill3DRect(x + 5, y + 5, 10, 20, false);  
            // 4.����Բ��  
            g.fillOval(x + 5, y + 10, 10, 10);  
            // 5.������  
            g.drawLine(x + 10, y + 15, x + 10, y);  
            break;  
        case 1:  
            // ��Ͳ����  
            // ��������ľ���  
            g.fill3DRect(x, y, 30, 5, false);  
            // ��������ľ���  
            g.fill3DRect(x, y + 15, 30, 5, false);  
            // �����м�ľ���  
            g.fill3DRect(x + 5, y + 5, 20, 10, false);  
            // ����Բ��  
            g.fillOval(x + 10, y + 5, 10, 10);  
            // ������  
            g.drawLine(x + 15, y + 10, x + 30, y + 10);  
            break;  
        case 2:  
            // ��Ͳ����  
            // 1.������ߵľ���  
            g.fill3DRect(x, y, 5, 30, false);  
            // 2.�����ұߵľ���  
            g.fill3DRect(x + 15, y, 5, 30, false);  
            // 3.�����м����  
            g.fill3DRect(x + 5, y + 5, 10, 20, false);  
            // 4.����Բ��  
            g.fillOval(x + 5, y + 10, 10, 10);  
            // 5.������  
            g.drawLine(x + 10, y + 15, x + 10, y + 30);  
            break;  
        case 3:  
            // ��Ͳ����  
            // ��������ľ���  
            g.fill3DRect(x, y, 30, 5, false);  
            // ��������ľ���  
            g.fill3DRect(x, y + 15, 30, 5, false);  
            // �����м�ľ���  
            g.fill3DRect(x + 5, y + 5, 20, 10, false);  
            // ����Բ��  
            g.fillOval(x + 10, y + 5, 10, 10);  
            // ������  
            g.drawLine(x + 15, y + 10, x, y + 10);  
  
        }  
  
    }  
  
    @Override  
    public void keyTyped(KeyEvent e) {  
        // TODO Auto-generated method stub  
  
    }  
  
    // �����´��� a d w s  
    @Override  
    public void keyPressed(KeyEvent e) {  
        if (e.getKeyCode() == KeyEvent.VK_W) {  
            // �����ҵ�̹�˵ķ���  
            // ����  
            this.hero.setDirect(0);  
            this.hero.moveUp();  
            System.out.println("��");  
            // this.repaint();  
        } else if (e.getKeyCode() == KeyEvent.VK_D) {  
            // ����  
            this.hero.setDirect(1);  
            this.hero.moveRight();  
        } else if (e.getKeyCode() == KeyEvent.VK_S) {  
            // ����  
            this.hero.setDirect(2);  
            this.hero.moveDown();  
            System.out.println("��");  
            // this.repaint();  
        } else if (e.getKeyCode() == KeyEvent.VK_A) {  
            // ����  
            this.hero.setDirect(3);  
            this.hero.moveLeft();  
        }  
          
        //�ж�����Ƿ���J  
        if(e.getKeyCode()==KeyEvent.VK_J){  
            //����  
            this.hero.shotEnemy();  
        }  
          
          
        // �����ػ�panel  
        this.repaint();  
    }  
  
    @Override  
    public void keyReleased(KeyEvent e) {  
    }  
  
    @Override  
    public void run() {  
        //ÿ��100����ȥ�ػ�  
        while(true){  
            try {  
                Thread.sleep(100);  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
            //�ػ�  
            this.repaint();  
        }  
    }  
}
//̹����,����  
class Tank {  
  // ��ʾ̹�˵ĺ�����  
  int x = 0;  
  // ̹�˵�������  
  int y = 0;  

  // ̹�˷���  
  // 0��ʾ�� 1��ʾ�� 2��ʾ�� 3��ʾ��  
  int direct = 0;  
    
  int color;    

  // ̹�˵��ٶ�  
  int speed = 1;  
    
    

  public int getColor() {  
      return color;  
  }  

  public void setColor(int color) {  
      this.color = color;  
  }  

  public int getSpeed() {  
      return speed;  
  }  

  public void setSpeed(int speed) {  
      this.speed = speed;  
  }  

  public int getDirect() {  
      return direct;  
  }  

  public void setDirect(int direct) {  
      this.direct = direct;  
  }  

  public Tank(int x, int y) {  
      this.x = x;  
      this.y = y;  
  }  

  public int getX() {  
      return x;  
  }  

  public void setX(int x) {  
      this.x = x;  
  }  

  public int getY() {  
      return y;  
  }  

  public void setY(int y) {  
      this.y = y;  
  }  

}  
//���˵�̹��  
class EnemyTank extends Tank{  
  public EnemyTank(int x,int y){  
      super(x, y);  
  }  
    
}  

//�ӵ���  
class Shot implements Runnable{  
  int x;  
  int y;  
  int direct;  
  int speed=1;  
  //�Ƿ񻹻���  
  boolean isLive=true;  
    
  public Shot(int x,int y,int direct){  
      this.x=x;  
      this.y=y;  
      this.direct=direct;  
  }  
  @Override  
  public void run() {  
      while(true){  
          try {  
              Thread.sleep(50);  
          } catch (Exception e) {  
              // TODO: handle exception  
          }  
            
          switch(direct){  
          case 0:  
              //��  
              y-=speed;  
              break;  
          case 1:  
              //��  
              x+=speed;  
              break;  
          case 2:  
              //��  
              y+=speed;  
              break;  
          case 3:  
              x-=speed;  
              break;  
          }  
          //�ӵ���ʱ����  
          System.out.println("�ӵ����꣺x="+x+"y=:"+y);  
            
          //�жϸ��ӵ��Ƿ�������Ե  
          if(x<0 || x>400 || y<0 || y>300){  
              this.isLive=false;  
              break;  
          }  
            
      }  
  }     
}  




//�ҵ�̹��  
class Hero extends Tank {  
    
  //�ӵ�  
  Shot s=null;  

  public Hero(int x, int y) {  
      super(x, y);  
  }  
    
  //����  
  public void shotEnemy(){          
        
      switch(this.direct){  
      case 0:  
          s=new Shot(x+10,y,0);             
          break;  
      case 1:  
          s=new Shot(x+30,y+10,1);  
          break;  
      case 2:  
          s=new Shot(x+10,y+30,2);  
          break;  
      case 3:  
          s=new Shot(x,y+10,3);  
          break;  
      }  
        
      //�����ӵ��߳�  
      Thread t=new Thread(s);  
      t.start();        
  }  
    
    

  // ̹�������ƶ�,�������С  
  public void moveUp() {  
      y -= speed;  
  }  

  // �����ƶ�  
  public void moveRight() {  
      x += speed;  
  }  

  // �����ƶ�  
  public void moveDown() {  
      this.y += this.speed;  
  }  

  // �����ƶ�  
  public void moveLeft() {  
      x -= speed;  
  }  
} 