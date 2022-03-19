package com.StackAndQueue;
/*
 * 利用单链表实现栈
 * head->top
 */
import com.xianxingbiao.*;

public class LinkedStack <T>{
	
	private LinkList<T> list;
	//构造一个空栈
	public LinkedStack()
	{
		list=new LinkList<T>();
	}
	//入栈
	//输入：新节点的value值
	public void push(T value)
	{
		list.add(value, list.size());
	}
	//出栈
	//输出：栈顶元素
	public T pop()
	{
		return list.remove(list.size());
	}
	//判断栈空
	public boolean IsEmpty()
	{
		return (list.size()==0);
	}
}






