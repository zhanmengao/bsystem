package com.xianxingbiao;

/*
 * ���ħ��ʦ��������
 */

public class Magic {
	//����13����,������13���Ƶ�ѭ������
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
				//ͷ�ڵ�
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
		//�����������γ���ƣ����õ�������
		for(int i=0;i<br.length;i++)
		{
			Brand now=br[i];
			//��ǰ�ƶ��Լ��ĵ�����
			for(int j=0;j<now.getFace();j++)
			{
				//������� ����ǰ��һ�������ӵ�ǰ�ƶ�
				MyHead=m.jump(MyHead);
			}
			//��ʱ����Ŀ���λ
			MyHead.setFace(now.getFace());
			System.out.println(now.getFace()+"�� ����");
		}
		
		
		//���ƽ��� ��ӡ����˳��
		System.out.print("����˳��Ϊ��");
		for(int i=0;i<br.length;i++)
		{
			head=(Brand) head.getNextNode();
			System.out.print(head.getFace()+"->");
		}
	}
	//�����ڵ�ĺ���(�ݹ���ã�ֱ���ɲ���ڵ㲻Ϊ��)
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
//һ������
class Brand extends Node1{
	private int face;

	public int getFace() {
		return face;
	}

	public void setFace(int face) {
		this.face = face;
	}
}
