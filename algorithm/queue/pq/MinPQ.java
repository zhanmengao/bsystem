package com.PQ;
//根节点最小优先队列：二叉堆实现
public class MinPQ<K extends Comparable<K>> implements AbPQ<K>{
	private K pq[];
	private int N;
	public MinPQ(int maxNum)
	{
		pq=(K[]) new Object[maxNum+1];
		N=0;
	}

	@Override
	public void insert(K k) {
		//插入到底部
		pq[++N]=k;
		//上浮
		swim(N-1);
		
	}
	//上浮
	private void swim(int j)
	{
		//如果没到顶且j小于父节点
		while(j>1 && less(j,j/2))
		{
			//交换
			swap(j,j/2);
			j/=2;
		}
	}
	//下沉
	private void sink(int i)
	{
		//如果还没有到底s
		while(2*i<=N)
		{
			int j=i*2;
			//找到它的孩子里面最小的
			if(less(j+1,j) && j<N)
			{
				j++;
			}
			//如果i比它最小的孩子小
			if(less(i,j))
			{
				break;
			}
			//否则 交换
			swap(i,j);
			i=j;
		}
	}
	//返回pq[i]是否小于pq[j]
	private boolean less(int i,int j)
	{
		return pq[i].compareTo(pq[j])<0;
	}
	private void swap(int i,int j)
	{
		K temp=pq[i];
		pq[i]=pq[j];
		pq[j]=temp;
	}

	@Override
	public K M() {
		return pq[1];
	}

	@Override
	public K del() {
		K root=pq[1];
		//尾部和他交换
		swap(1,N--);
		//下沉
		sink(1);
		pq[N+1]=null;
		return root;
	}

	@Override
	public boolean isEmpty() {
		return N==0;
	}

	@Override
	public int size() {
		return N;
	}

}
