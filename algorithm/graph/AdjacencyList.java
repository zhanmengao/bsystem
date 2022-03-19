package com.Graph;
/*
 * ͼ���ڽӱ�ṹ
 */

import java.util.ArrayList;
import java.util.LinkedList;
import com.StackAndQueue.*;

//�ڽӱ���
public class AdjacencyList implements Graph{
	private final int []id;//��������
	private ArrayList<Edge>[] adj;//��ÿ���������ıߵ�����
	private ArrayList<Edge> edges;//���б�
	private boolean marks[];//�������
	private int size;
	private int E;
	public AdjacencyList(int size)
	{
		id=new int[size];
		adj=(ArrayList<Edge>[])new ArrayList[size];
		marks=new boolean[size];
		this.size=size;
		E=0;
		edges=new ArrayList<AdjacencyList.Edge>();
	}
	
	class Edge{
		private int v;
		private int w;
		private float weight;
		public Edge(int v,int w)
		{
			this.v=v;
			this.w=w;
			weight=1;
		}
		public Edge(int v,int w,float weight)
		{
			this.v=v;
			this.w=w;
			this.weight=weight;
		}
		public int getV()
		{
			return v;
		}
		public int get(int v)
		{
			if(this.v==v)
			{
				return w;
			}else if(v==w)
			{
				return v;
			}
			else 
				throw new NullPointerException();
		}
	}
	//ǰ�����
	@Override
	public void DFS() {
		//����ķ���
		marks=new boolean[size];
		DFS(0);
		
	}
	//�Ӹ��ڵ㿪ʼ
	private void DFS(int v)
	{
		//��Ϊд������
		System.out.println(v+"�ѱ���");
		//��Ϊ�Ѿ�����
		marks[v]=true;
		//�ҵ��������ӵ�
		for(int i=0;i<adj[v].size();i++)
		{
			//ȡ����
			Edge e=adj[v].get(i);
			//ȡ����
			int w=e.get(v);
			if(marks[w]==false)
			{
				DFS(w);
			}
		}
		
	}

	@Override
	public void GFS() {
		if(id.length==0 || adj==null)
		{
			throw new NullPointerException();
		}
		//��ʼ����������
		marks=new boolean[size];
		//���涥��Ķ���
		LinkedQueue<Integer> queue=new LinkedQueue<Integer>();
		queue.add(0);
		while(!queue.IsEmpty())
		{
			//ȡ����һ������
			int v=queue.remove();
			//����Ϊ�ѱ���
			marks[v]=true;
			//�ҵ���ǰ�ڵ���ڽӵ�
			for(int i=0;i<adj[v].size();i++)
			{
				//ȡ����
				Edge e=adj[v].get(i);
				//ȡ����
				int w=e.get(v);
				if(marks[w]==false)
				{
					queue.add(w);
				}
			}
			
		}
		
	}

	@Override
	public int V() {
		return id.length;
	}

	@Override
	public int E() {
		return E;
	}

	@Override
	public void addEdge(int v, int w) {
		if(v<0 || v>=size || w<0 || w>=size || v==w)
		{
			throw new RuntimeException();
		}
		E++;
		if(v>w)
		{
			adj[v].add(new Edge(v,w));
			edges.add(new Edge(v,w));
		}else{
			adj[w].add(new Edge(w,v));	
			edges.add(new Edge(w,v));
		}
	}

	@Override
	//���ص�ǰ�ڵ����б�
	public ArrayList<Edge> adj(int v) {
		return adj[v];
	}
	@Override
	//�������б�
	public ArrayList<Edge> edges() {
		// TODO Auto-generated method stub
		return edges;
	}



	
}
