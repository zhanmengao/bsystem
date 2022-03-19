package com.StackAndQueue;
import java.io.BufferedReader;
import com.xianxingbiao.*;
import java.io.IOException;
import java.io.InputStreamReader;

/*
 * 用单链表生成一个队列
 */
import com.xianxingbiao.*;

public class LinkedQueue<T>{
	private LinkList<T> list;
	public LinkedQueue()
	{
		list=new LinkList<T>();
	}
	public int Size() {
		return list.size();
	}
	public boolean IsEmpty()
	{
		return (list.isEmpty());
	}
	//插入
	//输入：新节点
	public void add(T value)
	{
		list.add(value, list.size());
	}
	//删除
	//输出：队首
	public T remove()
	{
		return list.remove(list.size());	
	}
	//清空队列
	public void cleanQueue()
	{
		list=new LinkList<T>();
	}
	//逆置队列
	public void niZhi()
	{
		DyyStack stack=new DyyStack();
		//入栈
		while(!this.IsEmpty())
		{
			stack.Push(this.remove());
		}
		//出队
		while(!stack.IsEmpty())
		{
			this.add((T)stack.pop());
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//编写一个链队列，任意输入一串字符，以#作为结束标志，然后将队列中的元素显示到屏幕上。
	public static void main(String args[]) throws Exception
	{
		LinkedQueue queue=new LinkedQueue();
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String []in=br.readLine().split("");
		//执行入队，再出队打印
		for(String myin:in)
		{
			queue.add(myin);
		}
		//出队，打印
		while(!queue.IsEmpty())
		{
			System.out.print(queue.remove());
		}
	}


}








