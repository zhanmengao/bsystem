package com.xianxingbiao;
/*
 * ���ܣ��ҵ�������м�ڵ�
 */
public class Select_Middle {
	public Node1 getMiddle(Node1 head)
	{
		Node1 slow=head;
		Node1 quick=head;
		while(quick!=null)
		{
			slow=slow.getNextNode();
			quick=quick.getNextNode().getNextNode();
		}
		return slow;
	}
}
