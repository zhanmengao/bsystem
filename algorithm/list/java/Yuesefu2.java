package com.xianxingbiao;
/*
 * �����ǿԼɪ������
 */

public class Yuesefu2 {
	static //����ѭ������
	People Parray[]=null;
	public Yuesefu2(int size,int bound)
	{
		Parray=new People[size];
		for(int i=0;i<size;i++)
		{
			People people=new People(bound,i);
			System.out.println(i+"����ң�����������"+people.psw);
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
		int CountPeople=5;//������N
		int bound=5;//ȡֵ����M���Լ�ÿ��ȡֵ
		Yuesefu2 yuesefu=new Yuesefu2(CountPeople, bound);
		People people=Parray[0];//ȡ����һ����
		int jishuqi=0;
		while(jishuqi!=CountPeople)
		{
			//�ҵ�ÿ�εĵ�bound����
			People last=people;
			for(int i=1;i<bound;i++)
			{
				last=people;
				people=(People) people.getNextNode();
			}
			//�˳�ѭ����ɱ�������
			bound=people.psw;
			jishuqi++;
			System.out.println(people.index+"����� ������");
			
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




