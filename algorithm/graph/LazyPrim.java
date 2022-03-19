package com.Graph;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;

import com.Graph.AdjacencyList.Edge;
import com.StackAndQueue.*;

public class LazyPrim {
	private boolean peaks[];//顶点布尔类，用于检查顶点是否已经在最小生成树中
	private LinkedQueue edges;//最小生成树的边
	private PriorityQueue<Edge> pq;
	
	
	//生成最小生成树：每次选择距离最小的边，生成一条路径（局部最优解）：保证每个节点遍历到即可
	//输入：图
	public LazyPrim(Graph graph)
	{
		//初始化操作
		peaks=new boolean[graph.V()];
		//最小生成树的边
		edges=new LinkedQueue();
		//当前可选边的优先队列
		pq=new PriorityQueue<Edge>();
		
		//先访问第一个节点
		visit(graph, 0);
		//开始找每个最优秀的边
		while(!pq.isEmpty())
		{
			//取出优先队列第一条边
			Edge edge=pq.remove();
			int v=edge.getV();
			int w=edge.get(v);
			//检查它的两个顶点
			if(peaks[v]==true && peaks[w]==true)
			{
				//不添加
				continue;
			}
			//添加边到优先队列
			edges.add(edge);
			//找到没有访问过的顶点
			if(peaks[v]==false)
				visit(graph,v);
			if(peaks[w]==false)
				visit(graph,w);
		}
		
	}
	//功能：添加指定节点到最小生成树
	public void visit(Graph g,int v)
	{
		//遍历标记
		peaks[v]=true;
		//找到它的所有邻接边
		ArrayList<Edge> arr=g.adj(v);
		for(int i=0;i<arr.size();i++)
		{
			//取出边
			Edge e=arr.get(i);
			//如果另一个顶点没有被访问过
			if(peaks[e.get(v)]==false)
			{
				//把它的边添加到优先队列
				pq.add(e);
			}
		}
	}
	
	
	
	
	
	
	
	
	
}
