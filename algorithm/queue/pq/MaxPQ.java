package com.PQ;
//优先队列：二叉堆实现
public class MaxPQ<K extends Comparable<K>> implements AbPQ<K>{
	private K pq[];
	private int N;//当前元素个数
	public MaxPQ(int maxN)
	{
		pq=(K[]) new Object[maxN+1];
		N=0;
	}
	@Override
	public void insert(K k) {
		//添加到末尾
		pq[++N]=k;
		//k上浮
		swim(N-1);		
	}
	//将pq[k]上浮
	private void swim(int k)
	{
		//如果k的父亲比他小
		while(k>1 && less(k/2,k))
		{
			//交换
			swap(k/2,k);
			//更新K 继续向上比较
			k/=2;			
		}
	}
	//下沉
	private void sink(int k)
	{
		//如果还没有到底
		while(2*k<=N)
		{
			//拿到它的左孩子
			int j=k*2;
			if(j<N && less(j,j+1))
				j++;
			//如果k不比孩子中最大的小，则循环结束
			if(!less(k,j))
			{
				break;
			}
			//否则交换k和它最大的孩子,并更新k以便下一次下沉
			swap(k,j);
			k=j;
		}
	}
	//返回pq[i]是否小于pq[j]
	private boolean less(int i,int j)
	{
		//如果返回负整数，则pq[i]小于pq[j]成立
		return pq[i].compareTo(pq[j])<0;
	}
	private void swap(int i,int j)
	{
		K temp=pq[i];
		pq[i]=pq[j];
		pq[j]=temp;
	}
	@Override
	public final K M() {
		return pq[1];
	}

	@Override
	public K del() {
		//拿到根节点内存块的引用
		K root=pq[1];
		//交换尾节点和根节点
		swap(1,N--);
		//尾巴（现在的根节点）节点下沉
		sink(1);
		return root;
		
	}
	public static void sort(Comparable[] a)
	{
		
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
