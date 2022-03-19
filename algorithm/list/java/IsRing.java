package com.xianxingbiao;

public final class IsRing {
	//方法1：步长法
	public boolean function1(Node1 head)
	{
		Node1 fromlast = head;
		Node1 fromhead=head;
		//开始走
		int long1=0;
		while(fromlast!=null)
		{
			fromhead=head;//初始化fromhead的对象
			fromlast=fromlast.getNextNode();
			long1++;//每走一步，步长++
			
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
	
	//方法2：快慢指针法
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
