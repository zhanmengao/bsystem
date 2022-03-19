package com.Graph;

import java.util.ArrayList;

public class ListDigraph implements Digraph{
	private final int id[];
	private ArrayList<Edge> adj[];
	//�ߵļ���
	private ArrayList<Edge> edges;
	class Edge{
		private int from;
		private int to;
		private float weight;
		public Edge(int from,int to)
		{
			this.from=from;
			this.to=to;
			this.weight=1;
		}
		public Edge(int from,int to,float weight)
		{
			this.from=from;
			this.to=to;
			this.weight=weight;
		}
		public int from()
		{
			return from;
		}
		public int to()
		{
			return to;
		}
		public float weight()
		{
			return weight;
		}
		
	}
	//��ʼ��һ���ޱ�ͼ
	public ListDigraph(int size) {
		id=new int[size];
		adj=(ArrayList<Edge>[])new ArrayList[size];
		edges=new ArrayList<ListDigraph.Edge>();
	}
	@Override
	public int V() {
		return id.length;
	}

	@Override
	public int E() {
		return edges.size();
	}

	@Override
	public void addEdge(int from, int to) {
		if(from==to || from<0 || to<0 || from>=E() || to>=E())
		{
			//����
			throw new NullPointerException();
		}
		//��ӱ�
		adj[from].add(new Edge(from,to));
		edges.add(new Edge(from,to));
		
	}

	@Override
	public ArrayList<Edge> adj(int v) {
		return adj[v];
		
	}

	@Override
	public Digraph reverse() {
		Digraph dig=new ListDigraph(E());
		//������
		for(int i=0;i<edges.size();i++)
		{
			//ȡ��ÿһ����
			Edge e=edges.get(i);
			//�����ɷ���
			dig.addEdge(e.from, e.to);
		}
		
		return dig;
	}

}






