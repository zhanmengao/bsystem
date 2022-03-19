package com.symbolTable;
//��ʽ���ű�
public class ListST<K extends Comparable<K>,V> implements ST<K,V>{
	private Node head;//ָ������ͷ���
	private Node reaf;//ָ������β�ͽڵ�
	private int size;
	
	//��ֵ����
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
	//��key����ָ����
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
	
	//����µļ�ֵ��
	public void put(K key,V value)
	{
		if(this.head==null || this.reaf==null)
		{
			throw new NullPointerException();
		}
		//���Ҹ����ļ�
		Node now=head;
		for(int i=0;i<this.size;i++)
		{
			if(now.key.equals(key))
			{
				throw new Error("ɢ�����Ѵ��ڸ�key!");
			}
			now=now.next;
		}
		//û�� ����뵽β��
		Node newNode=new Node(key,value,null);
		//������β����Ӽ�
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







