package com.Graph;

import java.util.ArrayList;
import com.PQ.*;

import com.Graph.ListDigraph.Edge;

public class Dijkstra implements SP{
	//索引优先队列：存放每个顶点及其优先权
	private IndexMinPQ<Float> pq;
	//到每个顶点的最短路径
	private float []distTo;
	//到每个顶点的最短路径上最后一条边
	private Edge edges[];
	
	//迪杰斯特拉算法构造最短路径
	//输入：图，起点
	public Dijkstra(Digraph g,int start)
	{
		//初始化
		distTo=new float[g.V()];
		edges=new Edge[g.V()];
		pq=new IndexMinPQ<Float>(g.V());
		//设置距离为最大
		for(int i=0;i<g.V();i++)
		{
			//设置每个为正无穷大
			distTo[i]=Float.POSITIVE_INFINITY;
		}
		//设置到自己为0
		distTo[start]=0;
		//添加当前顶点到优先队列
		pq.insert(start,0f);
		
		//初始化完成 开始添
		while(!pq.isEmpty())
		{
			//将该顶点加入最短路径，并且更新原先的图（最短路径）
			relax(g,pq.delMin());
		}
		
	}
	
	private void relax(Digraph g,int v)
	{
		//取出该顶点每条边，看该顶点的加入是否使得distTo有所改变
		ArrayList<Edge> list=g.adj(v);
		for(Edge e:list)
		{
			//松弛每条边
			int w=e.to();	
			if(distTo[w]>distTo[v]+e.weight())
			{
				//如果到v再到w距离更短
				distTo[w]=distTo[v]+e.weight();
				edges[w]=e;
				//如果优先队列有这条边
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
