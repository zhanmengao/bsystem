package com.Graph;
/*
 * �����̤��������
 * �������ݹ�
 */

public class Horse {
	int X=5;
	int Y=5;
	int tag=0;
	int chess[][]=new int[X][Y];
	//���ܣ�ʵ����̤����
	//����:��ǰλ��(x,y) ��ǰ����tag
	public void mataqipan(Integer x,Integer y,int tag)
	{
		//��ֹ����,����Ѿ�������
		if(tag==X*Y)
		{
			return;
		}
		int count=0;//������
		boolean b=false;
		//��8��������ȥ����
		while(count<=7 || b==false)
		{
			b=false;
			//��������
			b=next(x,y,count++);
			//���ĳһ�γɹ���
			if(b==true)
			{
				tag++;
				//�ٴ�Ѱ���µ�λ��
				mataqipan(x, y, tag);
			}
		}
		if(b==false){
			//���ʧ���ˣ����λ���޽⣬�᷵�ص�ǰһ�ε���(��˷)
			chess[x][y]=0;
			tag--;
		}
	}
	//��̽������ķ���
	//���룺��ǰλ��(x,y)�����巽��way
	public boolean next(Integer x,Integer y,int way)
	{
		switch(way)
		{
			case 0:
				if( x+2<=X-1 && y-1>=0 && chess[x+2][y-1]==0 )
				{
					x = x + 2;
					y = y - 1;
					return true;
				}
				break;

			case 1:
				if( x+2<=X-1 && y+1<=Y-1 && chess[x+2][y+1]==0)
				{
					x = x + 2;
					y = y + 1;
					return true;
				}
				break;

			case 2:
				if( x+1<=X-1 && y-2>=0 && chess[x+1][y-2]==0 )
				{
					x = x + 1;
					y = y - 2;
					return true;
				}
				break;

			case 3:
				if( x+1<=X-1 && y+2<=Y-1 && chess[x+1][y+2]==0 )
				{
					x = x + 1;
					y = y + 2;
					return true;
				}
				break;

			case 4:
				if( x-2>=0 && y-1>=0 && chess[x-2][y-1]==0 )
				{
					x = x - 2;
					y = y - 1;
					return true;
				}
				break;

			case 5:
				if( x-2>=0 && y+1<=Y-1 && chess[x-2][y+1]==0 )
				{
					x = x - 2;
					y = y + 1;
					return true;
				}
				break;

			case 6:
				if( x-1>=0 && y-2>=0 && chess[x-1][y-2]==0)
				{
					x = x - 1;
					y = y - 2;
					return true;
				}
				break;

			case 7:
				if( x-1>=0 && y+2<=Y-1 && chess[x-1][y+2]==0 )
				{
					x = x - 1;
					y = y + 2;
					return true;
				}
				break;
		}
		return false;
	}

}






