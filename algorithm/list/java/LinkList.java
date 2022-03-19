package com.xianxingbiao;

import javax.management.RuntimeErrorException;

//功能：管理LinkList
public class LinkList<T> {
	private  Node1<T> head;//指向头结点
	private Node1<T> reaf;//指向尾部节点
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
	//功能：插入节点
	//输入:头结点，要插入的节点序列，插入的位置
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
		//找到目标位置
		Node1<T> now=head;
		//执行第0次循环前，处于第0个位置->我们要插入到第position个位置，应该拿到第position-1个位置元素的引用
		for(int i=0;i<position-1;i++)
		{
			now=now.getNextNode();
			if(now==null)
				throw new RuntimeException();
		}
		//插入
		newNode.setNextNode(now.getNextNode());
		now.setNextNode(newNode);
		size++;
	}
	//删除节点
	//输入：删除节点的位置
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
			//遍历
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
	//判断是否为空
	public boolean isEmpty()
	{
		return (size<=0);
	}
	//功能：获取链表长度
	//输出：链表长度(int)
	public int size()
	{
		return size;
	}
	//功能：获取指定节点
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
	
	//拿到倒数第i个元素
	public T getBottonIndex(int index)
	{
		if(index==this.size)
		{
			return reaf.getValue();
		}
		//计算下标
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
	//拿到链表交点
	public Node1 getNodical(LinkList<T> link)
	{
		
		if(link==null || link.size==0 || link.head==null)
		{
			throw new RuntimeException();
		}
		Node1 linkNow=link.head;
		Node1 thisNow=head;
		//较长指针移动d步
		if(this.size<link.size())
		{
			int shortOf=link.size-this.size;
			//参数指针移动插值位
			for(int i=0;i<shortOf;i++)
			{
				linkNow=linkNow.getNextNode();
			}
		}else{
			int shortOf=this.size-link.size;
			//参数指针移动插值位
			for(int i=0;i<shortOf;i++)
			{
				thisNow=thisNow.getNextNode();
			}	
		}
		//开始同步后移
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
	//逆置单链表
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
	//定义节点
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





