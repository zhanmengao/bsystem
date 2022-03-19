package com.SortSeek;

public class Sort {
	static Sort so1=new Sort();
	static int size=100000;
	static int a[]=new int [size];
	public static void main(String[] args)  {
		//Calendar cal=Calendar.getInstance();
		so1.stochastic(size);
		long time1=System.currentTimeMillis();
		//...
		
		long time2=System.currentTimeMillis();
		System.out.println("��ʱ"+(time2-time1));
		

	}
		
	//ʵ�����ɲ����������
	private static final int[] stochastic(int size)
	{
		for(int i=0;i<size;i++)
		{
			a[i]=(int) (Math.random()*100);
		}
		return a;
	}
	//дһ����ӡ����ķ���
	private static final void OutArray(int array[])
	{
		System.out.print("����������Ϊ ");
		for(int i=0;i<array.length;i++)
		{
			System.out.print(array[i]+" ");
		}
		System.out.println();
	}

	//ʵ��ð����������һ�����飬���һ������������
	public final void bubble(int array[])
	{
		//���ѭ������������Ҫ�ŵ��ǵڼ���������0��ʼֱ��length-1
		for(int i=0;i<array.length;i++)
		{
			//�ڲ�ѭ������������Ҫ�ŵ���Ҫ���ڼ������Ƚ�
			for(int j=i+1;j<array.length;j++)
			{
				//������ڱȽϵ����Ⱥ���������򽻻����ǵ�λ�ã���С��������ǰ��
				if(array[i]>array[j])
				{
					int temp=array[j];
					array[j]=array[i];
					array[i]=temp;
				}
			}
		}
		//so1.OutArray(array);
	}
	//ʵ��ѡ������,����һ�����飬�������������
	public final void select(int array[])
	{
		for(int i=0;i<array.length;i++)
		{
			//����Ϊ��ǰ����������С
			int MinVal=array[i];
			int MinIndex=i;
			for(int j=i+1;j<array.length;j++)
			{
				//����ǰ��С��ֵ�������ֵ���бȽϣ�ѡ����С��
				if(MinVal>array[j])
				{
					MinVal=array[j];
					MinIndex=j;
				}
			}
			//����λ��
			int temp=array[i];
			array[i]=MinVal;
			array[MinIndex]=temp;
		}
		//so1.OutArray(array);

	}
	//ʵ�ֲ�����������һ�����飬�������������
	public final void insert(int array[])
	{
		//˼�룺�����һ����Ϊ������������������λ�ò���
		//�ӵڶ�������ʼ��λ�ò���
		for(int i=1;i<array.length;i++)
		{
			int InsertVal=array[i];//����Ҫ�����Ԫ��
			int CompareIndex=i-1;//�����һ�������Ƚϵ����±�
			while(CompareIndex>=0&&InsertVal<array[CompareIndex])//�������鲻��������Ҫ�����Ԫ�ر�ǰһ��Ԫ��С
			{
				//�����������ȥ��Ԫ�غ��ƣ���һ�λḲ��array[i],Ȼ��ڶ��λḲ��array[i-1]������ʵ���ϱ����ǵ�ֻ��Insert������������������
				array[CompareIndex+1]=array[CompareIndex];
				//�������ʺ�����λ��
				CompareIndex--;
			}
			//�ҵ�λ����
			array[CompareIndex+1]=InsertVal;
		}
		//so1.OutArray(array);
		
	}
	//��������
	public static void sort(int array[],int lo,int hi)
	{
		if(lo<=hi)
			return;
		//���key�±�
		int i=getKey(array, lo, hi);
		//�ݹ�
		int mid=lo+(hi-lo)/2;
		sort(array,lo,mid);
		sort(array,mid+1,hi);
	}
	private static int getKey(int array[],int lo,int hi)
	{
		//���ȡ��һ������Ϊkey
		int index=lo+(int)Math.random()*(hi-lo);
		//���������
		int dual=array[index];
		array[index]=array[lo];
		array[lo]=array[index];
		//����Key
		int x=array[lo];
		int i=lo;
		//������ǰ����
		for(int j=lo + 1;j<hi;j++)
		{
			//�ұȹؼ�ֵС����
			if(array[j]<x)
			{
				i++;
				//exch array[j]],array[i]
				int temp=array[j];
				array[j]=array[i];
				array[i]=temp;
			}
		}
		//��������λ��lo��i(���һ����xС����)
		int temp=array[lo];
		array[lo]=array[i];
		array[i]=temp;
		return i;
	}
	
	//�鲢����
	public static void mergeSort(int array[],int lo,int hi)
	{
		if(lo>=hi)
			return;
		//�з�ÿ������
		mergeSort(array, lo, lo+(hi-lo)/2);
		mergeSort(array, lo+(hi-lo)/2+1, hi);
		merge(array,lo,hi);
	}
	private static void merge(int array[],int lo,int hi)
	{
		int i=lo;//�����������
		int j=lo+(hi-lo)/2+1;//�Ұ���������
		int a[]=new int[array.length];
		//���Ƹ�����
		for(int k=lo;k<hi;k++)
		{
			a[k]=array[k];
		}
		//�鲢
		for(int k=lo;k<hi;k++)
		{
			//�����������
			if(i>lo+(hi-lo)/2)
				//��j
				array[k]=a[j++];
			else if(j>hi)
				array[k]=a[i++];
			//��û�н�������С��
			else if(a[i]<a[j])
				array[k]=a[i++];
			else
				array[k]=a[j++];
			
		}
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
