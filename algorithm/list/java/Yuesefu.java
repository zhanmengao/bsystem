package com.xianxingbiao;

import java.util.LinkedList;

/*
 * ѭ������
 */

public class Yuesefu {
	public static void main(String main[])
	{
		MakeLoopList(41);
	}
	
	
	//����һ��ѭ������
	public static void MakeLoopList(int length)
	{
		Node1 nodes[]=new Node1[41];
		//�ȹ���һ������
		for(int i=0;i<length;i++)
		{
			Node1<Integer> newnode=new Node1<Integer>();
			newnode.setValue(i+1);
			nodes[i]=newnode;
			//�м�ڵ�
			if(i==0)
			{
				continue;
			}
			nodes[i-1].setNextNode(newnode);
			System.out.println(nodes[i-1].getValue()+" ������ "+newnode.getValue());
			if(i==40){
				//β�ڵ�
				//��39ָ��40
				nodes[40].setNextNode(nodes[0]);
				System.out.println(nodes[40].getValue()+" ������ "+nodes[0].getValue());
			}
		}
		//�����������б�
		int count=1;
		Node1 now=nodes[0];//��1�ſ�ʼ��ʱ
		Node1 lastnode=null;
		while(now.getValue()!=null)
		{
			//��ʼ����
			lastnode=now; 
			now=now.getNextNode();
			count++;//����
			if(count==3){
				//����
				System.out.println(now.getValue()+"����");
				Node1 nextnode=now.getNextNode();
				lastnode.setNextNode(nextnode);
				now.setValue(null);
				count=1;//���¿�ʼ����
				now=nextnode;//�µ�һ�ű���  nowΪ֮ǰ��now.next
			}
		}
		
	}
}




