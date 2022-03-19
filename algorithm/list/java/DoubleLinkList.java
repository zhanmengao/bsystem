package com.xianxingbiao;

class DoubleWayListManage{
	//ʵ�ֱ���˫����
	public int size(Node2 head)
	{
		int count=0;
		Node2 now=head;
		while(now!=null)
		{
			now=now.getNext();//��ʱΪnow������λ��Ϊ1->count=1
			count++;
		}
		return count;
	}
	//ʵ��˫����Ĳ��� 
	//����:ͷ�ڵ㣬����Ԫ�أ�����λ��
	//�����ͷ�ڵ�
	public Node2 add_toList(Node2 head,Node2 newnode,int position)
	{
		if(head==null)
		{
			return newnode;
		}
		//�ҵ�Ŀ��λ��
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
    //ʵ��˫�����ɾ������ ���룺ͷ�ڵ㣬ɾ���ڵ�λ�� �����ͷ�ڵ�
	public Node2 delete_inList(Node2 head,int position)
	{
		if(head==null)
		{
			return null;
		}
		//�ҵ�ɾ���ڵ��ǰ��
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
	//˫����ڵ�Ķ���
	private int value=0;
	private Node2 next=null;//������ָ��
	private Node2 previous=null;//������ָ��
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
