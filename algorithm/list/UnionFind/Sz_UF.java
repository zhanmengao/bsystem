package com.UnionFind;
/*
 * id数组表示其双亲节点
 * sz数组表示其所属类型的权
 */

public class Sz_UF {
	//节点（用数组下标代替）
	private int id[];
	//节点个数
	private int count;
	//所属集合的成员个数
	private int sz[];
	public Sz_UF(int size)
	{
		id=new int[size];
		count=size;
		sz=new int[size];
		for(int i=0;i<size;i++)
		{
			id[i]=i;
			sz[i]=1;
		}
		
	}
	//获取节点的归属
	public int Root(int p)
	{
		//如果不是自环（根节点）
		while(id[p]!=p)
		{
			p=id[p];//获取它的双亲节点，直到它的根节点（归属）
		}
		return p;
	}
	//判断两个节点是否同类
	public boolean connected(int p,int q)
	{
		return Root(p)==Root(q);
	}
	//合并两个节点
	public void Union(int p,int q)
	{
		int p_root=Root(p);
		int q_root=Root(q);
		if(p_root==q_root)
		{
			return;
		}else if(sz[p_root]<sz[q_root]){
			//把小的连到大的
			id[p_root]=q_root;
		}
		else{
			id[q_root]=p_root;
		}
	}

}
