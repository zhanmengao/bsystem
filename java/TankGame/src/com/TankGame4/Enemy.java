package com.TankGame4;

import java.util.Vector;

//����̹��Ҳ���ƶ��ͷ����ӵ�
//̹��һֱ���ػ棬���Ƿ�����Ҫһֱ�ػ棨5���ػ�һ�η���
//��Ϊ��ײʱ����̹���������Ϊ����������ΪӦ�ð���ײд������ķ�����
public class Enemy extends Tank4 implements Runnable
{
	public Enemy(int x, int y, int direct, int color) {
		super(x, y, direct, color);
	}
	
	Vector<Enemy> ve=new Vector<Enemy>();
	//��ȡMyPanel�ϵĵ���̹��
	public void setEenemy(Vector<Enemy> en)
	{
		this.ve=en;
	}
	//�������ǵ���ײ
	public void notoverlap()
	{
		//�û�ȡ��̹��������̹�˽��бȽ�
		for(int i=0;i<ve.size();i++)
		{
			//���Ⱥ����ȽϵĲ���������
			if(this!=ve.get(i))
			{
				//���ж��Ƿ���ײ������������ʾ���ˣ����룩
				if((this.x-ve.get(i).x<22) && (ve.get(i).x-this.x<22)
						&& (this.y-ve.get(i).y<22) && (ve.get(i).y-this.y<22))
				{
					//���������ײ ��ʹ���Ƿ���
					switch(this.direct)
					{
					case 1:
						this.direct=2;
						this.overlap=true;
						break;
					case 2:
						this.direct=1;
						this.overlap=true;
						break;
					case 3:
						this.direct=4;
						this.overlap=true;
						break;
					case 4:
						this.direct=3;
						this.overlap=true;
						break;					
					}
				}
			}
		}
	}
	public void run() {
		//ÿ4��ı�һ��̹�˷��򣺷���������һ���������1~4�����ֱ�������������ƶ�
		int time=0;
		while(true)
		{
			//�����ǰ������ͣ������Ϸ�߼���ͣ
			if(MyPanel5.isPaused==false)
			{
			//���̹������ �˳��߳�
			if(this.isLive==false)
			{
				break;
			}
			//ÿ���̼߳�����̹���Ƿ���ײ
			this.notoverlap();
			//���������ײ״̬���ı䷽�����ƶ�һ��
			if(this.overlap==true)
			{
				this.move(this.direct);
				this.overlap=false;
				continue;
			}
			//����̹���ƶ�
			if(time==0||time%10==0)
			{
				this.move(this.direct);
			}
			
			//ÿ3.5������ѡ��һ������
			if(time==0||time%350==0)
			{
				this.direct=(int)(1+Math.random()*3);
			}
			//����̹���Զ������ӵ�
			if(time==0||time%400==0)
			{
				this.biubiu();
			}

			//����̹����;֪��:��Ϊ��Ͳ��ͻ��������Ϊֻ��Ҫ�ж���Ͳ��ǰ��û�������߽�	
			switch(this.direct)
			{
			case 1:
				if(this.y-13<0)
				{
					this.direct=2;
				}
			    break;
			case 2:
				if(this.y+13>600)
				{
					this.direct=1;
				}
			    break;

			case 3:
				if(this.x-7<0)
					{
					this.direct=4;
					}
				break;
			case 4:
				if(this.x+7>600)
					{
					this.direct=3;
					}
				break;    
			}
			time++;
		}
			
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}	
	}		
}
