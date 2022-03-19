package com.xianxingbiao;
/*
 * 循环链表的功能
 */

public class LoopList {
	//功能：插入节点
	//输入：尾节点，新节点，插入位置
	//输出：尾节点
	public  static Node1 ListInsert(Node1 reaf,Node1 newnode,int position)
	{
		if(reaf==null && position<1)
		{
			throw new RuntimeException();
		}
		
		Node1 nownode=reaf;
		//从尾节点(逻辑位0)开始找position位置
		for(int i=0;i<position;i++)
		{
			nownode=nownode.getNextNode();
		}
		//此时nownode为前置节点
		Node1 nextnode=nownode.getNextNode();
		newnode.setNextNode(nextnode);
		nownode.setNextNode(newnode);

		return reaf;
	}
	//功能：删除节点
	//输入：尾节点，删除下标
	//输出：尾节点
	public static Node1 ListDelete(Node1 reaf,int position)
	{
		if(reaf==null)
		{
			return null;
		}
		Node1 lastnode = reaf;
		Node1 node=reaf;
		//找到目标节点
		for(int i=0;i<position;i++)
		{
			lastnode=node;
			node=node.getNextNode();
		}
		lastnode.setNextNode(node.getNextNode());
		node=null;
		return reaf;
	}
	//获取头节点
	public static Node1 getHead(Node1 reaf)
	{
		return reaf.getNextNode();
	}
	//获取链表长度
	public static int Size(Node1 node)
	{
		int size=0;
		while(node.getNextNode()!=node)
		{
			size++;
		}
		return size;	
	}
	
	
	
	
	

}