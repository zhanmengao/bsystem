package com.xianxingbiao;
/*
 * ѭ������Ĺ���
 */

public class LoopList {
	//���ܣ�����ڵ�
	//���룺β�ڵ㣬�½ڵ㣬����λ��
	//�����β�ڵ�
	public  static Node1 ListInsert(Node1 reaf,Node1 newnode,int position)
	{
		if(reaf==null && position<1)
		{
			throw new RuntimeException();
		}
		
		Node1 nownode=reaf;
		//��β�ڵ�(�߼�λ0)��ʼ��positionλ��
		for(int i=0;i<position;i++)
		{
			nownode=nownode.getNextNode();
		}
		//��ʱnownodeΪǰ�ýڵ�
		Node1 nextnode=nownode.getNextNode();
		newnode.setNextNode(nextnode);
		nownode.setNextNode(newnode);

		return reaf;
	}
	//���ܣ�ɾ���ڵ�
	//���룺β�ڵ㣬ɾ���±�
	//�����β�ڵ�
	public static Node1 ListDelete(Node1 reaf,int position)
	{
		if(reaf==null)
		{
			return null;
		}
		Node1 lastnode = reaf;
		Node1 node=reaf;
		//�ҵ�Ŀ��ڵ�
		for(int i=0;i<position;i++)
		{
			lastnode=node;
			node=node.getNextNode();
		}
		lastnode.setNextNode(node.getNextNode());
		node=null;
		return reaf;
	}
	//��ȡͷ�ڵ�
	public static Node1 getHead(Node1 reaf)
	{
		return reaf.getNextNode();
	}
	//��ȡ������
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