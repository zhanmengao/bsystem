package com.UnionFind;
/*
 * 并查集的第二种表示
 *id数组表示其上一节点
 */

public class QuickUnion {
	private int id[];
	private int count;
	public QuickUnion(int size)
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
		//如果不是自环（根节点）
		while(id[p]!=p)
		{
			p=id[p];//获取它的双亲节点
		}
		return p;
	}
	public boolean connected(int p,int q)
	{
		return Find(p)==Find(q);
	}
	public void Union(int p,int q)
	{
		int p_root=Find(p);
		int q_root=Find(q);
		if(p_root==q_root)
		{
			return;
		}else{
			id[p_root]=q_root;
		}
	}

}






