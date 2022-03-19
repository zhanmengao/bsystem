package com.Graph;
import java.util.PriorityQueue;
import com.UnionFind.*;
import com.Graph.AdjacencyList.Edge;
import com.StackAndQueue.*;
/*
 * ��³˹�����㷨 ������С������
 */

public class Kruskal {
	private LinkedQueue mst;
	
	public Kruskal(Graph graph)
	{
		mst=new LinkedQueue();//��С�������ߵĶ���
		PriorityQueue<Edge> pq=new PriorityQueue<Edge>();//��űߵ����ȶ���
		//������б�
		for(int i=0;i<graph.E();i++)
		{
			pq.add(graph.edges().get(i));
		}
		Sz_UF uf=new Sz_UF(graph.V());	//��ʼ��һ�����鼯
		//��ֹ������û�б��� ���� ���㹻��
		while(!pq.isEmpty() && mst.Size()<graph.V()-1)
		{
			//ȡ��Ȩ����С�ı�
			Edge e=pq.remove();
			//�õ���������
			int v=e.getV();
			int w=e.get(v);
			//�ж��Ƿ���ͬһ����
			if(uf.connected(v, w))
			{
				//�����Ѿ�����С�������ı�
				continue;
			}
			//���� ��������
			uf.Union(v, w);
			//��ӱߵ���С������
			mst.add(e);
			
		}
		
		
	}
}







