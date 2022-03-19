package com.Graph;
/*
 * 邻接矩阵
 */

import java.util.ArrayList;

import com.StackAndQueue.*;
public class AdjacencyMatrix {
	private Peak peaks[]=null;
	private double sides[][]=null;
	
	public Peak[] getPeaks() {
		return peaks;
	}
	public void setPeaks(Peak[] peaks) {
		this.peaks = peaks;
	}
	public double[][] getSides() {
		return sides;
	}
	public void setSides(double[][] sides) {
		this.sides = sides;
	}
	//构造图————邻接矩阵形式（有向图，无向图，网）
	//输入：顶点个数，从第一个顶点开始依次到其他顶点的距离，不可达则输入1000    若输入权则为网
	public AdjacencyMatrix(int length,double...distance )
	{
		peaks=new Peak[length];
		sides=new double[length][length];
		//将对角线置为0
		int count=0;
		//外层循环，一次循环初始化一个顶点
		for(int i=0;i<length;i++)
		{
			//内层循环，一次循环初始化该顶点的一条边
			for(int j=0;i<length;j++)
			{
				if(i==j)
				{
					sides[i][j]=0;
					continue;
				}
				sides[i][j]=distance[count++];
				
			}
		}
		
	}
	public void GFS()
	{
		GFS(peaks[0],peaks[0].getN());
	}
	//遍历图————深度优先算法
	//输入：第一个顶点，以及顶点的id
	private void GFS(Peak p1,int n)
	{
		//将当前顶点设为true，表示已经遍历
		p1.setIsOverride(true);
		//找到与它连接的点
		for(int j=0;j<peaks.length;j++)
		{
			//边不为0，则存在边
			if(sides[n][j]!=0)
			{
				//取出与之对应的点
				Peak p2=peaks[j];
				//看p2有没有检查过
				if(p2.isIsOverride()==false)
				{
					GFS(p2,j);			
				}
			}	
		}
		//如果本次找不到，则回返回到上一次递归的结果，
	}
	public void BFS()
	{
		BFS(peaks[0]);
	}
	//广度优先遍历算法
	//输入：一个顶点
	private void BFS(Peak peak)
	{
		LinkedQueue queue=new LinkedQueue();
		//当前节点入队列
		queue.add(peak);
		//不断从队列中取出节点，把他们的子节点入队列
		//每次循环，将一个顶点出队列，将与他相连的所有节点入队列
		while(!queue.IsEmpty())
		{
			Peak p=(Peak) queue.remove();
			p.setIsOverride(true);
			int n=p.getN();
			System.out.println(n+" 遍历了！");
			//组织它的相连点入队列
			for(int i=0;i<sides.length;i++)
			{
				if(sides[n][i]!=0 && peaks[n].isIsOverride()==false)
				{
					//入队列
					queue.add(peaks[i]);
				}
			}
		}	
	}
	

	
}
//顶点类
class Peak{
	private Object data;//顶点名称
	private boolean IsOverride=false;//遍历标记
	private int n;//序号

	public int getN() {
		return n;
	}

	public void setN(int n) {
		this.n = n;
	}

	public Object getData() {
		return data;
	}

	public void setData(Object data) {
		this.data = data;
	}

	public boolean isIsOverride() {
		return IsOverride;
	}

	public void setIsOverride(boolean isOverride) {
		IsOverride = isOverride;
	}
	
}

