package com.Graph;
/*
 * �ڽӾ���
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
	//����ͼ���������ڽӾ�����ʽ������ͼ������ͼ������
	//���룺����������ӵ�һ�����㿪ʼ���ε���������ľ��룬���ɴ�������1000    ������Ȩ��Ϊ��
	public AdjacencyMatrix(int length,double...distance )
	{
		peaks=new Peak[length];
		sides=new double[length][length];
		//���Խ�����Ϊ0
		int count=0;
		//���ѭ����һ��ѭ����ʼ��һ������
		for(int i=0;i<length;i++)
		{
			//�ڲ�ѭ����һ��ѭ����ʼ���ö����һ����
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
	//����ͼ����������������㷨
	//���룺��һ�����㣬�Լ������id
	private void GFS(Peak p1,int n)
	{
		//����ǰ������Ϊtrue����ʾ�Ѿ�����
		p1.setIsOverride(true);
		//�ҵ��������ӵĵ�
		for(int j=0;j<peaks.length;j++)
		{
			//�߲�Ϊ0������ڱ�
			if(sides[n][j]!=0)
			{
				//ȡ����֮��Ӧ�ĵ�
				Peak p2=peaks[j];
				//��p2��û�м���
				if(p2.isIsOverride()==false)
				{
					GFS(p2,j);			
				}
			}	
		}
		//��������Ҳ�������ط��ص���һ�εݹ�Ľ����
	}
	public void BFS()
	{
		BFS(peaks[0]);
	}
	//������ȱ����㷨
	//���룺һ������
	private void BFS(Peak peak)
	{
		LinkedQueue queue=new LinkedQueue();
		//��ǰ�ڵ������
		queue.add(peak);
		//���ϴӶ�����ȡ���ڵ㣬�����ǵ��ӽڵ������
		//ÿ��ѭ������һ����������У����������������нڵ������
		while(!queue.IsEmpty())
		{
			Peak p=(Peak) queue.remove();
			p.setIsOverride(true);
			int n=p.getN();
			System.out.println(n+" �����ˣ�");
			//��֯���������������
			for(int i=0;i<sides.length;i++)
			{
				if(sides[n][i]!=0 && peaks[n].isIsOverride()==false)
				{
					//�����
					queue.add(peaks[i]);
				}
			}
		}	
	}
	

	
}
//������
class Peak{
	private Object data;//��������
	private boolean IsOverride=false;//�������
	private int n;//���

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

