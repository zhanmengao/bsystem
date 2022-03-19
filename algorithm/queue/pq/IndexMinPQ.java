package com.PQ;
//�������ȶ��У�<����ֵ����>
public class IndexMinPQ<V extends Comparable<V>>{
	//��������
	private int pq[];
	//qp[i]��pq������Ϊi��Ԫ�ص��±�
	private int qp[];
	//Ԫ��ֵ����
	private V val[];
	//��ǰԪ�ظ���
	private int N;
	//�����������ȶ���
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
		//��ӵ�β�� Ȼ��ڵ��ϸ�
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
			//���k�Žڵ�С���丸�ڵ�
			if(less(k,k/2))
			{
				//�������ǵ�λ��
				swap(k,k/2);
				//����k
				k/=2;
			}else{
				break;
			}
		}
	}
	private void swap(int i,int j)
	{
		//����pq
		int tempK=pq[i];
		pq[i]=pq[j];
		pq[j]=tempK;
		//����qp
		int tempQP=qp[i];
		qp[i]=qp[j];
		qp[j]=tempQP;
		//����val
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
		//�滻β����k��Ԫ��
		swap(N,k);
		//β����Ϊ��
		pq[N]=Integer.MIN_VALUE;
		val[N]=null;
		N--;
		//k��Ԫ���³�
	}
	private boolean less(int i,int j)
	{
		return val[i].compareTo(val[j])<0;
	}
	//�³������������ڵ���ں��ӽڵ� ���³�
	private void sink(int k)
	{
		while(k*2<N)
		{
			//�ҵ��������������е���Сֵ
			int j=2*k;
			if(less(j+1,j))
			{
				j++;
			}
			//�����С�ĺ��ӱ���С���򽻻�
			if(less(j,k))
			{
				swap(j,k);
				k=j;
			}
		}
	}
	public V min()
	{
		//��С��Ԫ���Ǹ��ڵ�
		return val[1];
	}
	public int minIndex()
	{
		return pq[1];
	}
	public int delMin()
	{
		int temp=pq[1];
		//��β�ͽڵ��Ƶ�ͷ��
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
