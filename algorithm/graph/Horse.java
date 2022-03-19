package com.Graph;
/*
 * 解决马踏棋盘问题
 * 方法：递归
 */

public class Horse {
	int X=5;
	int Y=5;
	int tag=0;
	int chess[][]=new int[X][Y];
	//功能：实现马踏棋盘
	//输入:当前位置(x,y) 当前次数tag
	public void mataqipan(Integer x,Integer y,int tag)
	{
		//终止条件,如果已经下满棋
		if(tag==X*Y)
		{
			return;
		}
		int count=0;//计数器
		boolean b=false;
		//按8个方向尝试去下棋
		while(count<=7 || b==false)
		{
			b=false;
			//尝试下棋
			b=next(x,y,count++);
			//如果某一次成功了
			if(b==true)
			{
				tag++;
				//再次寻找新的位置
				mataqipan(x, y, tag);
			}
		}
		if(b==false){
			//如果失败了，则该位置无解，会返回到前一次调用(回朔)
			chess[x][y]=0;
			tag--;
		}
	}
	//试探性下棋的方法
	//输入：当前位置(x,y)，下棋方向way
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






