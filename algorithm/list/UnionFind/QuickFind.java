package com.UnionFind;
//并查集第一种形式：id数组表示其父节点

public class QuickFind {
	private int[] id;//id数组
	private int count;//类型数量
	//构造并查集
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
			//遍历id，找到所有和p同集合元素
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








