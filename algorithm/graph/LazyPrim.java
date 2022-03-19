package com.Graph;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.PriorityQueue;

import com.Graph.AdjacencyList.Edge;
import com.StackAndQueue.*;

public class LazyPrim {
	private boolean peaks[];//���㲼���࣬���ڼ�鶥���Ƿ��Ѿ�����С��������
	private LinkedQueue edges;//��С�������ı�
	private PriorityQueue<Edge> pq;
	
	
	//������С��������ÿ��ѡ�������С�ıߣ�����һ��·�����ֲ����Ž⣩����֤ÿ���ڵ����������
	//���룺ͼ
	public LazyPrim(Graph graph)
	{
		//��ʼ������
		peaks=new boolean[graph.V()];
		//��С�������ı�
		edges=new LinkedQueue();
		//��ǰ��ѡ�ߵ����ȶ���
		pq=new PriorityQueue<Edge>();
		
		//�ȷ��ʵ�һ���ڵ�
		visit(graph, 0);
		//��ʼ��ÿ��������ı�
		while(!pq.isEmpty())
		{
			//ȡ�����ȶ��е�һ����
			Edge edge=pq.remove();
			int v=edge.getV();
			int w=edge.get(v);
			//���������������
			if(peaks[v]==true && peaks[w]==true)
			{
				//�����
				continue;
			}
			//��ӱߵ����ȶ���
			edges.add(edge);
			//�ҵ�û�з��ʹ��Ķ���
			if(peaks[v]==false)
				visit(graph,v);
			if(peaks[w]==false)
				visit(graph,w);
		}
		
	}
	//���ܣ����ָ���ڵ㵽��С������
	public void visit(Graph g,int v)
	{
		//�������
		peaks[v]=true;
		//�ҵ����������ڽӱ�
		ArrayList<Edge> arr=g.adj(v);
		for(int i=0;i<arr.size();i++)
		{
			//ȡ����
			Edge e=arr.get(i);
			//�����һ������û�б����ʹ�
			if(peaks[e.get(v)]==false)
			{
				//�����ı���ӵ����ȶ���
				pq.add(e);
			}
		}
	}
	
	
	
	
	
	
	
	
	
}
