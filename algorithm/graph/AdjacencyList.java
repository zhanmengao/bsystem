package com.Graph;
/*
 * 图的邻接表结构
 */

import java.util.ArrayList;
import java.util.LinkedList;
import com.StackAndQueue.*;

//邻接表类
public class AdjacencyList implements Graph{
	private final int []id;//顶点数组
	private ArrayList<Edge>[] adj;//与每顶点相连的边的数组
	private ArrayList<Edge> edges;//所有边
	private boolean marks[];//遍历标记
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
	//前序遍历
	@Override
	public void DFS() {
		//对外的方法
		marks=new boolean[size];
		DFS(0);
		
	}
	//从根节点开始
	private void DFS(int v)
	{
		//行为写在这里
		System.out.println(v+"已遍历");
		//置为已经遍历
		marks[v]=true;
		//找到它的连接点
		for(int i=0;i<adj[v].size();i++)
		{
			//取出边
			Edge e=adj[v].get(i);
			//取出点
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
		//初始化布尔数组
		marks=new boolean[size];
		//储存顶点的队列
		LinkedQueue<Integer> queue=new LinkedQueue<Integer>();
		queue.add(0);
		while(!queue.IsEmpty())
		{
			//取出第一个顶点
			int v=queue.remove();
			//设置为已遍历
			marks[v]=true;
			//找到当前节点的邻接点
			for(int i=0;i<adj[v].size();i++)
			{
				//取出边
				Edge e=adj[v].get(i);
				//取出点
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
	//返回当前节点所有边
	public ArrayList<Edge> adj(int v) {
		return adj[v];
	}
	@Override
	//返回所有边
	public ArrayList<Edge> edges() {
		// TODO Auto-generated method stub
		return edges;
	}



	
}
