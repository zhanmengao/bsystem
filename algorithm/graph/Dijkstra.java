package com.Graph;

import java.util.ArrayList;
import com.PQ.*;

import com.Graph.ListDigraph.Edge;

public class Dijkstra implements SP{
	//�������ȶ��У����ÿ�����㼰������Ȩ
	private IndexMinPQ<Float> pq;
	//��ÿ����������·��
	private float []distTo;
	//��ÿ����������·�������һ����
	private Edge edges[];
	
	//�Ͻ�˹�����㷨�������·��
	//���룺ͼ�����
	public Dijkstra(Digraph g,int start)
	{
		//��ʼ��
		distTo=new float[g.V()];
		edges=new Edge[g.V()];
		pq=new IndexMinPQ<Float>(g.V());
		//���þ���Ϊ���
		for(int i=0;i<g.V();i++)
		{
			//����ÿ��Ϊ�������
			distTo[i]=Float.POSITIVE_INFINITY;
		}
		//���õ��Լ�Ϊ0
		distTo[start]=0;
		//��ӵ�ǰ���㵽���ȶ���
		pq.insert(start,0f);
		
		//��ʼ����� ��ʼ��
		while(!pq.isEmpty())
		{
			//���ö���������·�������Ҹ���ԭ�ȵ�ͼ�����·����
			relax(g,pq.delMin());
		}
		
	}
	
	private void relax(Digraph g,int v)
	{
		//ȡ���ö���ÿ���ߣ����ö���ļ����Ƿ�ʹ��distTo�����ı�
		ArrayList<Edge> list=g.adj(v);
		for(Edge e:list)
		{
			//�ɳ�ÿ����
			int w=e.to();	
			if(distTo[w]>distTo[v]+e.weight())
			{
				//�����v�ٵ�w�������
				distTo[w]=distTo[v]+e.weight();
				edges[w]=e;
				//������ȶ�����������
				if(pq.contains(w))
				{
					pq.change(w, distTo[w]);
				}else{
					pq.insert(w, distTo[w]);
				}
			}
		}
	}

	@Override
	public double distTo(int v) {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public boolean hasPathTo(int v) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Iterable<Edge> pathTo(int b) {
		// TODO Auto-generated method stub
		return null;
	}

}
