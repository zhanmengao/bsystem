package com.tree;

import java.util.PriorityQueue;

import com.StackAndQueue.*;

public class HuffmanTree {
	BinaryNode huffmanroot=null;
	PriorityQueue<BinaryNode> pq=null;
	//���ܣ������������
	//���룺�������еĶ��������
	public HuffmanTree(BinaryNode[] binarynodes)
	{
		pq=new PriorityQueue<BinaryNode>();
		//���ڵ��������ȶ���
		for(BinaryNode binarynode:binarynodes)
		{
			pq.add(binarynode);
		}
		//2��ÿ�δӶ�����ȡ��Ԫ�أ��ٷ���һ��Ԫ��
		while(pq.size()>1)
		{
			BinaryNode newnode_small=pq.remove();
			BinaryNode newnode_big=pq.remove();
			//��������ϣ���Ϊ�½ڵ�
			BinaryNode newnode=new BinaryNode();
			newnode.setData((Integer)newnode_big.getData()+(Integer)newnode_small.getData());
			newnode.setLeft(newnode_big);
			newnode.setRight(newnode_small);
			//���½ڵ㰴˳����뵽������
			pq.add(newnode);
		}
		huffmanroot=(BinaryNode) pq.remove();
		
	}

}
