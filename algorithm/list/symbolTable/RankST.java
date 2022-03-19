package com.symbolTable;
//有序数组的符号表
import javax.swing.text.DefaultEditorKit.CopyAction;

public class RankST<K extends Comparable<K>,V> implements ST<K,V>{
	private K[] keys;
	private V[] values;
	private int size;
	public RankST(int capacity)
	{
		keys=(K[])new Object[capacity];
		values=(V[])new Object[capacity];
		size=0;
	}
	//读取键值对
	public V get(K key)
	{
		//找到位置
		int index=rank(key);
		//如果存在
		if(key.compareTo(keys[index])==0)
		{
			return values[index];
		}
		return null;
	}
	//插入键值对
	public void put(K key,V value)
	{
		int index=rank(key);
		//如果存在
		if(key.compareTo(keys[index])==0)
		{
			throw new Error();
		}
		//不存在
		else{
			//腾位置
			for(int j=size;j>index;j--)
			{
				keys[j]=keys[j-1];
				values[j]=values[j-1];
			}
			keys[index]=key;
			values[index]=value;
			size++;
		}
	}
	//迭代形式二分查找：找到小于k的元素个数
	public int rank(K key)
	{
		if(key==null)
		{
			throw new RuntimeException();
		}
		int lo=0;int hi=size-1;
		while(lo<=hi)
		{
			int mid=lo+(hi-lo)/2;
			//与中间比较
			int cmp=key.compareTo(keys[mid]);
			//如果key在keys左边
			if(cmp<0)
			{
				hi=mid-1;	
			}else if(cmp>0)
			{
				lo=mid+1;
			}
			else{
				return mid;
			}
		}
		return lo;
	}
	public int size()
	{
		return size;
	}
	@Override
	public V remoove(K key) {
		if(key==null)
		{
			throw new RuntimeException();
		}
		V value=null;
		//找到键
		int index=rank(key);
		//判断是否存在
		if(key.compareTo(keys[index])!=0)
		{
			return null;
		}else{
			value=values[index];
			//前移
			for(int i=index;i<size-1;i++)
			{
				values[i]=values[i+1];
				keys[i]=keys[i+1];
			}
		}
		
		return value;
	}
	
}
