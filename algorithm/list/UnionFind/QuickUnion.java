package com.UnionFind;
/*
 * ���鼯�ĵڶ��ֱ�ʾ
 *id�����ʾ����һ�ڵ�
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
		//��������Ի������ڵ㣩
		while(id[p]!=p)
		{
			p=id[p];//��ȡ����˫�׽ڵ�
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






