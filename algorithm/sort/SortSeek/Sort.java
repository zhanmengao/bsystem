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
		System.out.println("用时"+(time2-time1));
		

	}
		
	//实现生成参数个随机数
	private static final int[] stochastic(int size)
	{
		for(int i=0;i<size;i++)
		{
			a[i]=(int) (Math.random()*100);
		}
		return a;
	}
	//写一个打印数组的方法
	private static final void OutArray(int array[])
	{
		System.out.print("排序后的数组为 ");
		for(int i=0;i<array.length;i++)
		{
			System.out.print(array[i]+" ");
		}
		System.out.println();
	}

	//实现冒泡排序，输入一个数组，输出一个排序后的书序
	public final void bubble(int array[])
	{
		//外层循环，决定现在要排的是第几个数，从0开始直到length-1
		for(int i=0;i<array.length;i++)
		{
			//内层循环，决定现在要排的数要跟第几个数比较
			for(int j=i+1;j<array.length;j++)
			{
				//如果现在比较的数比后面的数大，则交换它们的位置，把小的数不断前移
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
	//实现选择排序,输入一个数组，输出排序后的数组
	public final void select(int array[])
	{
		for(int i=0;i<array.length;i++)
		{
			//我认为当前的数就是最小
			int MinVal=array[i];
			int MinIndex=i;
			for(int j=i+1;j<array.length;j++)
			{
				//将当前最小的值与后续的值进行比较，选出最小的
				if(MinVal>array[j])
				{
					MinVal=array[j];
					MinIndex=j;
				}
			}
			//交换位置
			int temp=array[i];
			array[i]=MinVal;
			array[MinIndex]=temp;
		}
		//so1.OutArray(array);

	}
	//实现插入排序，输入一个数组，输出排序后的数组
	public final void insert(int array[])
	{
		//思想：定义第一个数为有序表，后面的数不断找位置插入
		//从第二个数开始找位置插入
		for(int i=1;i<array.length;i++)
		{
			int InsertVal=array[i];//定义要插入的元素
			int CompareIndex=i-1;//定义第一个与它比较的数下标
			while(CompareIndex>=0&&InsertVal<array[CompareIndex])//控制数组不溢出，如果要插入的元素比前一个元素小
			{
				//把这个被比下去的元素后移（第一次会覆盖array[i],然后第二次会覆盖array[i-1]……，实际上被覆盖的只有Insert，其他数都被后移了
				array[CompareIndex+1]=array[CompareIndex];
				//继续找适合它的位置
				CompareIndex--;
			}
			//找到位置了
			array[CompareIndex+1]=InsertVal;
		}
		//so1.OutArray(array);
		
	}
	//快速排序
	public static void sort(int array[],int lo,int hi)
	{
		if(lo<=hi)
			return;
		//获得key下标
		int i=getKey(array, lo, hi);
		//递归
		int mid=lo+(hi-lo)/2;
		sort(array,lo,mid);
		sort(array,mid+1,hi);
	}
	private static int getKey(int array[],int lo,int hi)
	{
		//随机取得一个数作为key
		int index=lo+(int)Math.random()*(hi-lo);
		//换到最左边
		int dual=array[index];
		array[index]=array[lo];
		array[lo]=array[index];
		//设置Key
		int x=array[lo];
		int i=lo;
		//遍历当前数组
		for(int j=lo + 1;j<hi;j++)
		{
			//找比关键值小的数
			if(array[j]<x)
			{
				i++;
				//exch array[j]],array[i]
				int temp=array[j];
				array[j]=array[i];
				array[i]=temp;
			}
		}
		//交换索引位置lo和i(最后一个比x小的数)
		int temp=array[lo];
		array[lo]=array[i];
		array[i]=temp;
		return i;
	}
	
	//归并排序
	public static void mergeSort(int array[],int lo,int hi)
	{
		if(lo>=hi)
			return;
		//切分每个数组
		mergeSort(array, lo, lo+(hi-lo)/2);
		mergeSort(array, lo+(hi-lo)/2+1, hi);
		merge(array,lo,hi);
	}
	private static void merge(int array[],int lo,int hi)
	{
		int i=lo;//左半数组索引
		int j=lo+(hi-lo)/2+1;//右半数组索引
		int a[]=new int[array.length];
		//复制该数组
		for(int k=lo;k<hi;k++)
		{
			a[k]=array[k];
		}
		//归并
		for(int k=lo;k<hi;k++)
		{
			//索引结束检测
			if(i>lo+(hi-lo)/2)
				//填j
				array[k]=a[j++];
			else if(j>hi)
				array[k]=a[i++];
			//都没有结束，填小的
			else if(a[i]<a[j])
				array[k]=a[i++];
			else
				array[k]=a[j++];
			
		}
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}
