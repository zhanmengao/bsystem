package com.xianxingbiao;

public final class IsRing {
	//����1��������
	public boolean function1(Node1 head)
	{
		Node1 fromlast = head;
		Node1 fromhead=head;
		//��ʼ��
		int long1=0;
		while(fromlast!=null)
		{
			fromhead=head;//��ʼ��fromhead�Ķ���
			fromlast=fromlast.getNextNode();
			long1++;//ÿ��һ��������++
			
			int long2=0;
			while(fromhead!=fromlast)
			{
				fromhead=fromhead.getNextNode();
				long2++;
			}
			if(long1!=long2)
			{
				return true;
			}
		}
		return false;
	}
	
	//����2������ָ�뷨
	public boolean function2(Node1 head)
	{
		Node1 slower=head;
		Node1 quicker=head;
		while(quicker!=null)
		{
			slower=slower.getNextNode();
			quicker=quicker.getNextNode().getNextNode();
			if(slower==quicker)
			{
				return true;
			}
		}
		return false;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
}
