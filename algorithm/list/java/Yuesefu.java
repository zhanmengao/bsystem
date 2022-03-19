package com.xianxingbiao;

import java.util.LinkedList;

/*
 * 循环链表
 */

public class Yuesefu {
	public static void main(String main[])
	{
		MakeLoopList(41);
	}
	
	
	//构造一个循环链表
	public static void MakeLoopList(int length)
	{
		Node1 nodes[]=new Node1[41];
		//先构造一个链表
		for(int i=0;i<length;i++)
		{
			Node1<Integer> newnode=new Node1<Integer>();
			newnode.setValue(i+1);
			nodes[i]=newnode;
			//中间节点
			if(i==0)
			{
				continue;
			}
			nodes[i-1].setNextNode(newnode);
			System.out.println(nodes[i-1].getValue()+" 连接向 "+newnode.getValue());
			if(i==40){
				//尾节点
				//让39指向40
				nodes[40].setNextNode(nodes[0]);
				System.out.println(nodes[40].getValue()+" 连接向 "+nodes[0].getValue());
			}
		}
		//遍历出死亡列表
		int count=1;
		Node1 now=nodes[0];//从1号开始计时
		Node1 lastnode=null;
		while(now.getValue()!=null)
		{
			//开始计数
			lastnode=now; 
			now=now.getNextNode();
			count++;//报数
			if(count==3){
				//死亡
				System.out.println(now.getValue()+"死亡");
				Node1 nextnode=now.getNextNode();
				lastnode.setNextNode(nextnode);
				now.setValue(null);
				count=1;//重新开始计数
				now=nextnode;//新的一号报数  now为之前的now.next
			}
		}
		
	}
}




