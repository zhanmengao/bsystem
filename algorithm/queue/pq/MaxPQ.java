package com.PQ;
//���ȶ��У������ʵ��
public class MaxPQ<K extends Comparable<K>> implements AbPQ<K>{
	private K pq[];
	private int N;//��ǰԪ�ظ���
	public MaxPQ(int maxN)
	{
		pq=(K[]) new Object[maxN+1];
		N=0;
	}
	@Override
	public void insert(K k) {
		//��ӵ�ĩβ
		pq[++N]=k;
		//k�ϸ�
		swim(N-1);		
	}
	//��pq[k]�ϸ�
	private void swim(int k)
	{
		//���k�ĸ��ױ���С
		while(k>1 && less(k/2,k))
		{
			//����
			swap(k/2,k);
			//����K �������ϱȽ�
			k/=2;			
		}
	}
	//�³�
	private void sink(int k)
	{
		//�����û�е���
		while(2*k<=N)
		{
			//�õ���������
			int j=k*2;
			if(j<N && less(j,j+1))
				j++;
			//���k���Ⱥ���������С����ѭ������
			if(!less(k,j))
			{
				break;
			}
			//���򽻻�k�������ĺ���,������k�Ա���һ���³�
			swap(k,j);
			k=j;
		}
	}
	//����pq[i]�Ƿ�С��pq[j]
	private boolean less(int i,int j)
	{
		//������ظ���������pq[i]С��pq[j]����
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
		//�õ����ڵ��ڴ�������
		K root=pq[1];
		//����β�ڵ�͸��ڵ�
		swap(1,N--);
		//β�ͣ����ڵĸ��ڵ㣩�ڵ��³�
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
