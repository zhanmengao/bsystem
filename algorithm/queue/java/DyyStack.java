package com.StackAndQueue;

public class DyyStack{
	private int size;//容量
	private int top;//头部索引位置
	private Object array[];//容器
	//初始化
	public DyyStack()
	{
		this.size=size;
		array= new Object[5];
		top=-1;
	}
	//功能：判断栈满  O(1)
	private boolean IsFull()
	{
		if(this.top==this.size-1)
		{
			return true;
		}else{
			return false;
		}
	}
	//功能：判断栈空 O(1)
	public boolean IsEmpty()
	{
		return (this.top==-1);
	}
	
	//功能：入栈 O(1)
	//输入：新的栈顶
	public void Push(Object newnode)
	{
		if(this.IsFull())
		{
			//扩充
			Object[] newarray=new Object[size*2];
			System.arraycopy(array, 0, newarray, 0, size);
			size=size*2;
			array=newarray;
		}
		array[++top]=newnode;
	}
	//功能：出栈 O(1)
	//输出：栈顶元素
	public Object pop()
	{
		if(this.IsEmpty())
		{
			throw new RuntimeException();	
		}
		return array[top--];
	}
	//功能：重置栈 O(1)
	public void ClearStack(int newsize)
	{
		array=new Object[newsize];
	}
	//功能：销毁栈 O(1)
	public void MakeNullStack()
	{
		array=null;
	}
	//功能：计算栈的剩余容量 O(1)
	public int getNowSize()
	{
		return this.size-this.top;
	}
}
