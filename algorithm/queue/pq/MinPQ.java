package com.PQ;
//���ڵ���С���ȶ��У������ʵ��
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
		//���뵽�ײ�
		pq[++N]=k;
		//�ϸ�
		swim(N-1);
		
	}
	//�ϸ�
	private void swim(int j)
	{
		//���û������jС�ڸ��ڵ�
		while(j>1 && less(j,j/2))
		{
			//����
			swap(j,j/2);
			j/=2;
		}
	}
	//�³�
	private void sink(int i)
	{
		//�����û�е���s
		while(2*i<=N)
		{
			int j=i*2;
			//�ҵ����ĺ���������С��
			if(less(j+1,j) && j<N)
			{
				j++;
			}
			//���i������С�ĺ���С
			if(less(i,j))
			{
				break;
			}
			//���� ����
			swap(i,j);
			i=j;
		}
	}
	//����pq[i]�Ƿ�С��pq[j]
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
		//β����������
		swap(1,N--);
		//�³�
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
