package com.xianxingbiao;

/*
 * 解决魔术师发牌问题
 */

public class Magic {
	//构造13张牌,和容纳13张牌的循环链表
	static Brand br[]=new Brand[13];
	static Brand head=null;
	static Brand []temp=new Brand[13];
	public Magic()
	{
		for(int i=0;i<br.length;i++)
		{
			Brand brand=new Brand();
			brand.setFace(i+1);
			br[i]=brand;
			
			Brand now=new Brand();
			temp[i]=now;
			if(i==0){
				//头节点
				continue;
			}
			temp[i-1].setNextNode(now);
			if(i==12)
			{
				now.setNextNode(temp[0]);
			}
		}
	}
	
	
	public static void main(String args[])
	{
		Magic m=new Magic();
		head=temp[0];
		for(int i=0;i<temp.length;i++)
		{
			head=(Brand) head.getNextNode();
		}
		Brand MyHead=head;
		//从数组中依次抽出牌，放置到链表中
		for(int i=0;i<br.length;i++)
		{
			Brand now=br[i];
			//向前移动自己的点数步
			for(int j=0;j<now.getFace();j++)
			{
				//如果有牌 则往前移一步并忽视当前移动
				MyHead=m.jump(MyHead);
			}
			//此时来到目标空位
			MyHead.setFace(now.getFace());
			System.out.println(now.getFace()+"号 放入");
		}
		
		
		//放牌结束 打印牌组顺序
		System.out.print("牌组顺序为：");
		for(int i=0;i<br.length;i++)
		{
			head=(Brand) head.getNextNode();
			System.out.print(head.getFace()+"->");
		}
	}
	//跳过节点的函数(递归调用，直到可插入节点不为空)
	public Brand jump(Brand node)
	{
		node=(Brand) node.getNextNode();
		if(node.getFace()!=0)
		{
			node=jump(node);
		}
		return node;
	}

}
//一个牌类
class Brand extends Node1{
	private int face;

	public int getFace() {
		return face;
	}

	public void setFace(int face) {
		this.face = face;
	}
}
