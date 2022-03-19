package com.symbolTable;
//��������ķ��ű�
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
	//��ȡ��ֵ��
	public V get(K key)
	{
		//�ҵ�λ��
		int index=rank(key);
		//�������
		if(key.compareTo(keys[index])==0)
		{
			return values[index];
		}
		return null;
	}
	//�����ֵ��
	public void put(K key,V value)
	{
		int index=rank(key);
		//�������
		if(key.compareTo(keys[index])==0)
		{
			throw new Error();
		}
		//������
		else{
			//��λ��
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
	//������ʽ���ֲ��ң��ҵ�С��k��Ԫ�ظ���
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
			//���м�Ƚ�
			int cmp=key.compareTo(keys[mid]);
			//���key��keys���
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
		//�ҵ���
		int index=rank(key);
		//�ж��Ƿ����
		if(key.compareTo(keys[index])!=0)
		{
			return null;
		}else{
			value=values[index];
			//ǰ��
			for(int i=index;i<size-1;i++)
			{
				values[i]=values[i+1];
				keys[i]=keys[i+1];
			}
		}
		
		return value;
	}
	
}
