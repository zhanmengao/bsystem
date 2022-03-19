package com.StackAndQueue;
/*
 * 自增长型循环队列
 */

public class DyyLoopQueue {
	private Object array[];
	private int head;//指向下一个要出队的人
	private int reaf;//指向下一个可以插入元素的空间
	private int size;//队列的元素个数
	public DyyLoopQueue()
	{
		array=new Object[5];
		head=0;
		reaf=0;
	}
	public boolean IsEmpty()
	{
		return (head==reaf);
	}
	//入队列操作
	//输入：要插入队列的元素
	public void push(Object value)
	{
		//如果尾指针马上要追上头指针->扩充
		if((reaf+1)%array.length==head)
		{
			int size=array.length;
			//扩充
			Object[] newarray=new Object[size*2];
			System.arraycopy(array, 0, newarray, 0, size);
			size=size*2;
			array=newarray;
		}
		array[reaf]=value;
		//更新reaf
		reaf=(reaf+1)%(array.length);
		size++;
	}
	//出队列操作
	//输出：队首元素
	public Object pop()
	{
		if(IsEmpty())
		{
			throw new RuntimeException();
		}
		Object nowtop=array[head];
		//top指针后移一位 
		head=(head+1)%array.length;
		size--;
		
		return nowtop;
	}
	public int getSize() {
		return size;
	}
	//按升序插入到原先队列
	//输入：新节点值
	public void Insert_ASC(Object data)
	{
		//找到他的位置  
	}
}











