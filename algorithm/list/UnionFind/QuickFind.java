package com.UnionFind;
//���鼯��һ����ʽ��id�����ʾ�丸�ڵ�

public class QuickFind {
	private int[] id;//id����
	private int count;//��������
	//���첢�鼯
	public QuickFind(int size)
	{
		id=new int[size];
		count=size;
		for(int i=0;i<size;i++)
		{
			id[i]=i;
		}
	}
	public int Find(int p)
	{
		return id[p];
	}
	public boolean Connected(int p,int q)
	{
		return Find(p)==Find(q);
	}
	public void union(int p,int q)
	{
		if(Connected(p, q))
		{
			return;
		}
		else{
			int p_root=Find(p);
			int q_root=Find(q);
			//����id���ҵ����к�pͬ����Ԫ��
			for(int i=0;i<id.length;i++)
			{
				if(id[i]==p_root)
				{
					id[i]=q_root;
				}
			}
		}
		
		
	}
}








