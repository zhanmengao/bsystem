package com.PQ;
//索引优先队列：<排序值类型>
public class IndexMinPQ<V extends Comparable<V>>{
	//索引数组
	private int pq[];
	//qp[i]是pq中索引为i的元素的下标
	private int qp[];
	//元素值数组
	private V val[];
	//当前元素个数
	private int N;
	//构造索引优先队列
	public IndexMinPQ(int maxN)
	{
		pq=new int[maxN+1];
		for(int i=0;i<pq.length;i++)
		{
			pq[i]=Integer.MIN_VALUE;
			qp[i]=Integer.MIN_VALUE;
		}
		val=(V[]) new Object[maxN+1];
		N=0;
	}
	public boolean contains(int k)
	{
		return qp[k]!=Integer.MIN_VALUE;
	}
	public void insert(int k,V item)
	{
		//添加到尾部 然后节点上浮
		N++;
		pq[N]=k;
		val[N]=item;
		qp[k]=N;
		swim(N);
	}
	private void swim(int k)
	{
		while(k>1)
		{
			//如果k号节点小于其父节点
			if(less(k,k/2))
			{
				//交换他们的位置
				swap(k,k/2);
				//更改k
				k/=2;
			}else{
				break;
			}
		}
	}
	private void swap(int i,int j)
	{
		//交换pq
		int tempK=pq[i];
		pq[i]=pq[j];
		pq[j]=tempK;
		//交换qp
		int tempQP=qp[i];
		qp[i]=qp[j];
		qp[j]=tempQP;
		//交换val
		V tempV=val[i];
		val[i]=val[j];
		val[j]=tempV;		
	}
	public void change(int k,V item)
	{
		val[k]=item;
		swim(qp[k]);
		sink(qp[k]);
	}
	public void delete(int k)
	{
		//替换尾部和k号元素
		swap(N,k);
		//尾部置为空
		pq[N]=Integer.MIN_VALUE;
		val[N]=null;
		N--;
		//k号元素下沉
	}
	private boolean less(int i,int j)
	{
		return val[i].compareTo(val[j])<0;
	}
	//下沉操作：若根节点大于孩子节点 则下沉
	private void sink(int k)
	{
		while(k*2<N)
		{
			//找到它的两个孩子中的最小值
			int j=2*k;
			if(less(j+1,j))
			{
				j++;
			}
			//如果最小的孩子比她小，则交换
			if(less(j,k))
			{
				swap(j,k);
				k=j;
			}
		}
	}
	public V min()
	{
		//最小的元素是根节点
		return val[1];
	}
	public int minIndex()
	{
		return pq[1];
	}
	public int delMin()
	{
		int temp=pq[1];
		//将尾巴节点移到头部
		swap(1,N);
		pq[N]=Integer.MIN_VALUE;
		val[N]=null;
		N--;
		return temp;	
	}
	public boolean isEmpty()
	{
		return N==0;
	}
	public int size()
	{
		return N;
	}
}
