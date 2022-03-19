package com.Graph;
import java.util.PriorityQueue;
import com.UnionFind.*;
import com.Graph.AdjacencyList.Edge;
import com.StackAndQueue.*;
/*
 * 克鲁斯卡尔算法 构造最小生成树
 */

public class Kruskal {
	private LinkedQueue mst;
	
	public Kruskal(Graph graph)
	{
		mst=new LinkedQueue();//最小生成树边的队列
		PriorityQueue<Edge> pq=new PriorityQueue<Edge>();//存放边的优先队列
		//添加所有边
		for(int i=0;i<graph.E();i++)
		{
			pq.add(graph.edges().get(i));
		}
		Sz_UF uf=new Sz_UF(graph.V());	//初始化一个并查集
		//终止条件：没有边了 并且 边足够多
		while(!pq.isEmpty() && mst.Size()<graph.V()-1)
		{
			//取出权重最小的边
			Edge e=pq.remove();
			//拿到两个顶点
			int v=e.getV();
			int w=e.get(v);
			//判断是否在同一集合
			if(uf.connected(v, w))
			{
				//忽略已经在最小生成树的边
				continue;
			}
			//否则 连接他们
			uf.Union(v, w);
			//添加边到最小生成树
			mst.add(e);
			
		}
		
		
	}
}







