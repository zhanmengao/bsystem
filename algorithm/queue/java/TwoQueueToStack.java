package com.StackAndQueue;
import java.util.*;
public class TwoQueueToStack<T> {
	private LinkedQueue<T> queue1;
	private LinkedQueue<T> queue2;
	public TwoQueueToStack()
	{
		queue1=new LinkedQueue<T>();
		queue2=new LinkedQueue<T>();
	}
	public void Push(T obj)
	{
		queue1.add(obj);
	}
	public T Pop()
	{
		//栈先进后出->弹出队列中最后进入的元素
		return null;
	}
}
