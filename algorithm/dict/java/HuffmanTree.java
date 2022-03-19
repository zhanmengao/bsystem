package com.tree;

import java.util.PriorityQueue;

import com.StackAndQueue.*;

public class HuffmanTree {
	BinaryNode huffmanroot=null;
	PriorityQueue<BinaryNode> pq=null;
	//功能：构造哈夫曼树
	//输入：升序排列的二叉树结点
	public HuffmanTree(BinaryNode[] binarynodes)
	{
		pq=new PriorityQueue<BinaryNode>();
		//将节点排入优先队列
		for(BinaryNode binarynode:binarynodes)
		{
			pq.add(binarynode);
		}
		//2、每次从队列中取出元素，再放入一个元素
		while(pq.size()>1)
		{
			BinaryNode newnode_small=pq.remove();
			BinaryNode newnode_big=pq.remove();
			//将他们组合，作为新节点
			BinaryNode newnode=new BinaryNode();
			newnode.setData((Integer)newnode_big.getData()+(Integer)newnode_small.getData());
			newnode.setLeft(newnode_big);
			newnode.setRight(newnode_small);
			//将新节点按顺序插入到队列中
			pq.add(newnode);
		}
		huffmanroot=(BinaryNode) pq.remove();
		
	}

}
