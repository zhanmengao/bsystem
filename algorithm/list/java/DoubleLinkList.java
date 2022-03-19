package com.xianxingbiao;

class DoubleWayListManage{
	//实现遍历双链表
	public int size(Node2 head)
	{
		int count=0;
		Node2 now=head;
		while(now!=null)
		{
			now=now.getNext();//此时为now的索引位置为1->count=1
			count++;
		}
		return count;
	}
	//实现双链表的插入 
	//输入:头节点，插入元素，插入位置
	//输出：头节点
	public Node2 add_toList(Node2 head,Node2 newnode,int position)
	{
		if(head==null)
		{
			return newnode;
		}
		//找到目标位置
		Node2 last=head;
		for(int i=1;i<position;i++)
		{
			last=last.getNext();
		}
		Node2 next=last.getNext();
		
		last.setNext(newnode);
		newnode.setPrevious(last);
		next.setPrevious(newnode);
		newnode.setNext(next);
		return head;
	}
    //实现双链表的删除数据 输入：头节点，删除节点位置 输出：头节点
	public Node2 delete_inList(Node2 head,int position)
	{
		if(head==null)
		{
			return null;
		}
		//找到删除节点的前驱
		Node2 last=head;
		for(int i=1;i<position;i++)
		{
			last=last.getNext();
		}
		Node2 now=last.getNext();
		Node2 next=now.getNext();
		
		last.setNext(next);
		next.setPrevious(last);
		now=null;
		return head;
	}
	
}

class Node2 {
	//双链表节点的定义
	private int value=0;
	private Node2 next=null;//定义右指针
	private Node2 previous=null;//定义左指针
	public int getValue() {
		return value;
	}
	public void setValue(int value) {
		this.value = value;
	}
	public Node2 getNext() {
		return next;
	}
	public void setNext(Node2 next) {
		this.next = next;
	}
	public Node2 getPrevious() {
		return previous;
	}
	public void setPrevious(Node2 previous) {
		this.previous = previous;
	}
}
