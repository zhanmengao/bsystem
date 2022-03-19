package com.xianxingbiao;

import javax.management.RuntimeErrorException;

//���ܣ�����LinkList
public class LinkList<T> {
	private  Node1<T> head;//ָ��ͷ���
	private Node1<T> reaf;//ָ��β���ڵ�
	private int size;
	
	public Node1<T> getHead() {
		return head;
	}
	public Node1<T> getReaf() {
		return reaf;
	}
	public LinkList()
	{
		head=new Node1<T>();
		reaf=new Node1<T>();
		size=0;
	}
	//���ܣ�����ڵ�
	//����:ͷ��㣬Ҫ����Ľڵ����У������λ��
	public void add(T value,int position)
	{
		Node1<T> newNode=new Node1<T>();
		newNode.setValue(value);
		if(head==null|| size==0)
		{
			head=newNode;
			reaf=newNode;
			size++;
			return;
		}
		if(position==0)
		{
			newNode.setNextNode(head);
			head=newNode;
			size++;
			return;
		}
		if(position==size)
		{
			reaf.setNextNode(newNode);
			reaf=newNode;
			size++;
			return;
		}
		//�ҵ�Ŀ��λ��
		Node1<T> now=head;
		//ִ�е�0��ѭ��ǰ�����ڵ�0��λ��->����Ҫ���뵽��position��λ�ã�Ӧ���õ���position-1��λ��Ԫ�ص�����
		for(int i=0;i<position-1;i++)
		{
			now=now.getNextNode();
			if(now==null)
				throw new RuntimeException();
		}
		//����
		newNode.setNextNode(now.getNextNode());
		now.setNextNode(newNode);
		size++;
	}
	//ɾ���ڵ�
	//���룺ɾ���ڵ��λ��
	public T remove(int position)
	{
		if(position==1)
		{
			size--;
			T temp=head.getValue();
			head=head.getNextNode();
			return temp;
		}
		else{
			//����
			Node1 lastnode=head;
			for(int i=2;i<position;i++)		
			{
				lastnode=lastnode.getNextNode();
				if(lastnode.getNextNode()==null)
				{
					return null;
				}
			}
			Node1 position_node=lastnode.getNextNode();
			T temp=(T) position_node.getValue();
			position_node=null;
			lastnode.setNextNode(lastnode.getNextNode().getNextNode());
			size--;
			return temp;
		}	
	}
	//�ж��Ƿ�Ϊ��
	public boolean isEmpty()
	{
		return (size<=0);
	}
	//���ܣ���ȡ������
	//�����������(int)
	public int size()
	{
		return size;
	}
	//���ܣ���ȡָ���ڵ�
	public T get(int position)
	{
		if(position==1)
			return head.getValue();
		if(position==size)
		{
			return reaf.getValue();
		}
		Node1 nownode=head;
		for(int i=2;i<position;i++)
		{
			nownode=nownode.getNextNode();
			if(nownode==null)
				return null;
		}
		return (T) nownode.getValue();
	}
	
	//�õ�������i��Ԫ��
	public T getBottonIndex(int index)
	{
		if(index==this.size)
		{
			return reaf.getValue();
		}
		//�����±�
		int position=size-index;
		Node1 nowNode=head;
		for(int i=0;i<position;i++)
		{
			nowNode=nowNode.getNextNode();
		}
		return (T) nowNode.getValue();
	}
	public Node1 getTop()
	{
		return head;
	}
	//�õ�������
	public Node1 getNodical(LinkList<T> link)
	{
		
		if(link==null || link.size==0 || link.head==null)
		{
			throw new RuntimeException();
		}
		Node1 linkNow=link.head;
		Node1 thisNow=head;
		//�ϳ�ָ���ƶ�d��
		if(this.size<link.size())
		{
			int shortOf=link.size-this.size;
			//����ָ���ƶ���ֵλ
			for(int i=0;i<shortOf;i++)
			{
				linkNow=linkNow.getNextNode();
			}
		}else{
			int shortOf=this.size-link.size;
			//����ָ���ƶ���ֵλ
			for(int i=0;i<shortOf;i++)
			{
				thisNow=thisNow.getNextNode();
			}	
		}
		//��ʼͬ������
		while(!linkNow.getValue().equals(thisNow.getValue()))
		{
			if(linkNow==null || thisNow==null)
			{
				return null;
			}
			linkNow=linkNow.getNextNode();
			thisNow=thisNow.getNextNode();
		}
		return linkNow;
	}
	//���õ�����
	public void ReverseList()
	{
		Node1<T> temp=null;
		Node1<T> next=null;
		while(head!=null)
		{
			next=head.getNextNode();
			head.setNextNode(temp);
			temp = head;
			head=next;
		}
		head=temp;
	}
	
}

class Node1<T> {
	//����ڵ�
	private T value;
	private Node1 nextNode;
	
	public T getValue() {
		return value;
	}
	public void setValue(T value) {
		this.value = value;
	}
	public Node1 getNextNode() {
		return nextNode;
	}
	public void setNextNode(Node1 nextNode) {
		this.nextNode = nextNode;
	}
	
}





