package com.TankGame3;

/*
 * ���ܣ�̹�����ࣨ�ҷ�̹�ˣ��з�̹�ˣ�
 */
public class Hero3 extends Tank3{
	
	bullet bu=null;//����һ���ӵ�����
	
	//�ҵ�̹�˼̳��˸��๹�췽����Ҳ�ṩ��ʼ���꣬�Լ�һЩ���������Լ�������
	public Hero3(int x, int y,int direct,int color) 
	{
		super(x, y,direct,color);
		dx=2;
		dy=2;
	}
	
	//�ṩ����ķ�����1�����ݷ���ʵ����һ���ӵ�   2���ø��ӵ������߳�
	public void biubiu()
	{
		System.out.println("����");
		//����̹�˷��򣬸���ʼ�����ӵ�����ֵ
		switch(this.direct)
		{
		case 1:
			bu=new bullet(this.x,this.y-12,1);
			break;
		case 2:
			bu=new bullet(this.x,this.y+8,2);
			break;
		case 3:
			bu=new bullet(this.x-8,this.y,3);
			break;
		case 4:
			bu=new bullet(this.x+15,this.y,4);
			break;
		}
		//��ʵ������ɵĶ��������ӵ��߳�
		Thread t1=new Thread(bu);
		t1.start();

	}
	//�ṩ�ƶ��ķ���
	public void move(int direct)
	{
		switch(direct)
		{
		case 1:
			y-=dy;
			break;
		case 2:
			y+=dy;
			break;
		case 3:
			x-=dx;
			break;
		case 4:
			x+=dx;
			break;
		}
	}
}
class Enemy extends Tank3
{

	public Enemy(int x, int y, int direct, int color) {
		super(x, y, direct, color);
	}

}

