package com.xianxingbiao;
/*
 * 解决加强约瑟夫问题
 */

public class Yuesefu2 {
	static //构造循环链表
	People Parray[]=null;
	public Yuesefu2(int size,int bound)
	{
		Parray=new People[size];
		for(int i=0;i<size;i++)
		{
			People people=new People(bound,i);
			System.out.println(i+"号玩家，他的密码是"+people.psw);
			Parray[i]=people;
			if(i!=0){
				Parray[i-1].setNextNode(people);
			}
			if(i==size-1){
				people.setNextNode(Parray[0]);
			}
		}
	} 
	public static void main(String args[])
	{
		int CountPeople=5;//总人数N
		int bound=5;//取值上限M，以及每次取值
		Yuesefu2 yuesefu=new Yuesefu2(CountPeople, bound);
		People people=Parray[0];//取出第一个人
		int jishuqi=0;
		while(jishuqi!=CountPeople)
		{
			//找到每次的第bound个人
			People last=people;
			for(int i=1;i<bound;i++)
			{
				last=people;
				people=(People) people.getNextNode();
			}
			//退出循环，杀死这个人
			bound=people.psw;
			jishuqi++;
			System.out.println(people.index+"号玩家 死亡！");
			
			last.setNextNode(people.getNextNode());
			people=(People) people.getNextNode();
			
			
		}
		
	}

}

class People  extends Node1{
	public int psw=0;
	public int index=0;
	public People(int bound,int index)
	{
		psw=(int)(1+Math.random()*(bound-1));
		this.index=index;
	}
}




