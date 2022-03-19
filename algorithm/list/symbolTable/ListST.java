package com.symbolTable;
//链式符号表
public class ListST<K extends Comparable<K>,V> implements ST<K,V>{
	private Node head;//指向链表头结点
	private Node reaf;//指向链表尾巴节点
	private int size;
	
	//键值链表
	class Node{
		K key;
		V value;
		Node next;
		public Node(K key,V value,Node next)
		{
			this.key=key;
			this.next=next;
			this.value=value;
		}
		
	}
	public ListST()
	{
		size=0;
	}
	//按key查找指定键
	public V get(K key)
	{
		if(head==null)
		{
			return null;
		}
		Node now=head;
		while(now!=null)
		{
			if(now.key.equals(key))
			{
				return now.value;
			}
			now=now.next;	
		}
		return null;	
	}
	
	//添加新的键值对
	public void put(K key,V value)
	{
		if(this.head==null || this.reaf==null)
		{
			throw new NullPointerException();
		}
		//查找给定的键
		Node now=head;
		for(int i=0;i<this.size;i++)
		{
			if(now.key.equals(key))
			{
				throw new Error("散列中已存在该key!");
			}
			now=now.next;
		}
		//没有 则插入到尾部
		Node newNode=new Node(key,value,null);
		//在链表尾部添加键
		if(size==0)
		{
			head=newNode;
			reaf=newNode;
		}
		else{
			reaf.next=newNode;
			reaf=newNode;
		}
		size++;
	}
	@Override
	public int size() {
		return size;
	}
	@Override
	public V remoove(K key) {
		
		return null;
	}	
}







