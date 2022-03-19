package com.TankGame2;

import javax.swing.*;   

import java.awt.*;  
import java.awt.event.*;  
import java.util.*;  
  
  
//我的坦克大战游戏3.0版本  
/** 
 * 我的坦克可以上下左右移动 
 *  
 * @author Administrator 
 * 
 */  
  
public class HanTankGame3 extends JFrame {  
  
    MyPanel mp = null;  
  
    public static void main(String[] args) {  
        HanTankGame3 mtg = new HanTankGame3();  
    }  
  
    // 构造函数  
    public HanTankGame3() {  
        mp = new MyPanel();  
          
        //启动mp线程  
        Thread t=new Thread(mp);  
        t.start();        
          
        this.add(mp);  
        // 注册监听  
        this.addKeyListener(mp);  
  
        this.setSize(400, 300);  
        this.setVisible(true);  
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }  
  
}  
  
// 我的面板  
class MyPanel extends JPanel implements KeyListener,Runnable{  
  
    // 定义一个我的坦克  
    Hero hero = null;  
      
    //定义敌人的坦克组  
    Vector<EnemyTank> ets=new Vector<EnemyTank>();  
  
    //定义敌人坦克的数量  
    int enSize=3;     
      
    // 构造函数  
    public MyPanel() {  
        hero = new Hero(100, 100);        
        //初始化敌人的坦克  
        for(int i=0;i<enSize;i++){  
            //创建一辆敌人的坦克对象  
            EnemyTank et=new EnemyTank((i+1)*50,0);  
            et.setColor(0);  
            et.setDirect(2);  
            //加入到vector集合中  
            ets.add(et);  
        }  
    }  
  
    // 重写paint  
    public void paint(Graphics g) {  
        super.paint(g);  
        // 把背景色变成黑色  
        g.fillRect(0, 0, 400, 300);  
        g.setColor(Color.CYAN);  
  
        // 画出自己的坦克  
        this.drawTank(hero.getX(), hero.getY(), g, this.hero.direct, 1);  
          
        //画出子弹  
        if(hero.s!=null && hero.s.isLive==true){
            g.draw3DRect(hero.s.x,hero.s.y,1,1,false);  
        }  
          
        //画出敌人的坦克  
        for(int i=0;i<ets.size();i++){             
            this.drawTank(ets.get(i).getX(), ets.get(i).getY(), g, ets.get(i).getDirect(), 0);  
        }  
  
    }  
  
    // 画出坦克的函数  
    public void drawTank(int x, int y, Graphics g, int direct, int type) {  
        // 判断是什么类型的坦克  
        switch (type) {  
        case 0:  
            g.setColor(Color.CYAN);  
            break;  
        case 1:  
            g.setColor(Color.yellow);  
            break;  
        }  
  
        // 判断方向  
        switch (direct) {  
        // 向上  
        case 0:  
            // 画出我的坦克  
            // 1.画出左边的矩形  
            g.fill3DRect(x, y, 5, 30, false);  
            // 2.画出右边的矩形  
            g.fill3DRect(x + 15, y, 5, 30, false);  
            // 3.画出中间矩形  
            g.fill3DRect(x + 5, y + 5, 10, 20, false);  
            // 4.画出圆形  
            g.fillOval(x + 5, y + 10, 10, 10);  
            // 5.画出线  
            g.drawLine(x + 10, y + 15, x + 10, y);  
            break;  
        case 1:  
            // 炮筒向右  
            // 画出上面的矩形  
            g.fill3DRect(x, y, 30, 5, false);  
            // 画出下面的矩形  
            g.fill3DRect(x, y + 15, 30, 5, false);  
            // 画出中间的矩形  
            g.fill3DRect(x + 5, y + 5, 20, 10, false);  
            // 画出圆形  
            g.fillOval(x + 10, y + 5, 10, 10);  
            // 画出线  
            g.drawLine(x + 15, y + 10, x + 30, y + 10);  
            break;  
        case 2:  
            // 炮筒向下  
            // 1.画出左边的矩形  
            g.fill3DRect(x, y, 5, 30, false);  
            // 2.画出右边的矩形  
            g.fill3DRect(x + 15, y, 5, 30, false);  
            // 3.画出中间矩形  
            g.fill3DRect(x + 5, y + 5, 10, 20, false);  
            // 4.画出圆形  
            g.fillOval(x + 5, y + 10, 10, 10);  
            // 5.画出线  
            g.drawLine(x + 10, y + 15, x + 10, y + 30);  
            break;  
        case 3:  
            // 炮筒向左  
            // 画出上面的矩形  
            g.fill3DRect(x, y, 30, 5, false);  
            // 画出下面的矩形  
            g.fill3DRect(x, y + 15, 30, 5, false);  
            // 画出中间的矩形  
            g.fill3DRect(x + 5, y + 5, 20, 10, false);  
            // 画出圆形  
            g.fillOval(x + 10, y + 5, 10, 10);  
            // 画出线  
            g.drawLine(x + 15, y + 10, x, y + 10);  
  
        }  
  
    }  
  
    @Override  
    public void keyTyped(KeyEvent e) {  
        // TODO Auto-generated method stub  
  
    }  
  
    // 键按下处理 a d w s  
    @Override  
    public void keyPressed(KeyEvent e) {  
        if (e.getKeyCode() == KeyEvent.VK_W) {  
            // 设置我的坦克的方向  
            // 向上  
            this.hero.setDirect(0);  
            this.hero.moveUp();  
            System.out.println("上");  
            // this.repaint();  
        } else if (e.getKeyCode() == KeyEvent.VK_D) {  
            // 向右  
            this.hero.setDirect(1);  
            this.hero.moveRight();  
        } else if (e.getKeyCode() == KeyEvent.VK_S) {  
            // 向下  
            this.hero.setDirect(2);  
            this.hero.moveDown();  
            System.out.println("下");  
            // this.repaint();  
        } else if (e.getKeyCode() == KeyEvent.VK_A) {  
            // 向左  
            this.hero.setDirect(3);  
            this.hero.moveLeft();  
        }  
          
        //判断玩家是否按下J  
        if(e.getKeyCode()==KeyEvent.VK_J){  
            //开火  
            this.hero.shotEnemy();  
        }  
          
          
        // 必须重绘panel  
        this.repaint();  
    }  
  
    @Override  
    public void keyReleased(KeyEvent e) {  
    }  
  
    @Override  
    public void run() {  
        //每隔100毫秒去重绘  
        while(true){  
            try {  
                Thread.sleep(100);  
            } catch (InterruptedException e) {  
                e.printStackTrace();  
            }  
            //重绘  
            this.repaint();  
        }  
    }  
}
//坦克类,父类  
class Tank {  
  // 表示坦克的横坐标  
  int x = 0;  
  // 坦克的纵坐标  
  int y = 0;  

  // 坦克方向  
  // 0表示上 1表示右 2表示下 3表示左  
  int direct = 0;  
    
  int color;    

  // 坦克的速度  
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
//敌人的坦克  
class EnemyTank extends Tank{  
  public EnemyTank(int x,int y){  
      super(x, y);  
  }  
    
}  

//子弹类  
class Shot implements Runnable{  
  int x;  
  int y;  
  int direct;  
  int speed=1;  
  //是否还活着  
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
              //上  
              y-=speed;  
              break;  
          case 1:  
              //右  
              x+=speed;  
              break;  
          case 2:  
              //下  
              y+=speed;  
              break;  
          case 3:  
              x-=speed;  
              break;  
          }  
          //子弹何时死亡  
          System.out.println("子弹坐标：x="+x+"y=:"+y);  
            
          //判断该子弹是否碰到边缘  
          if(x<0 || x>400 || y<0 || y>300){  
              this.isLive=false;  
              break;  
          }  
            
      }  
  }     
}  




//我的坦克  
class Hero extends Tank {  
    
  //子弹  
  Shot s=null;  

  public Hero(int x, int y) {  
      super(x, y);  
  }  
    
  //开火  
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
        
      //启动子弹线程  
      Thread t=new Thread(s);  
      t.start();        
  }  
    
    

  // 坦克向上移动,纵坐标减小  
  public void moveUp() {  
      y -= speed;  
  }  

  // 向右移动  
  public void moveRight() {  
      x += speed;  
  }  

  // 向下移动  
  public void moveDown() {  
      this.y += this.speed;  
  }  

  // 向左移动  
  public void moveLeft() {  
      x -= speed;  
  }  
} 