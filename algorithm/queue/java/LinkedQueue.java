package com.StackAndQueue;
import java.io.BufferedReader;
import com.xianxingbiao.*;
import java.io.IOException;
import java.io.InputStreamReader;

/*
 * �õ���������һ������
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
	//����
	//���룺�½ڵ�
	public void add(T value)
	{
		list.add(value, list.size());
	}
	//ɾ��
	//���������
	public T remove()
	{
		return list.remove(list.size());	
	}
	//��ն���
	public void cleanQueue()
	{
		list=new LinkList<T>();
	}
	//���ö���
	public void niZhi()
	{
		DyyStack stack=new DyyStack();
		//��ջ
		while(!this.IsEmpty())
		{
			stack.Push(this.remove());
		}
		//����
		while(!stack.IsEmpty())
		{
			this.add((T)stack.pop());
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//��дһ�������У���������һ���ַ�����#��Ϊ������־��Ȼ�󽫶����е�Ԫ����ʾ����Ļ�ϡ�
	public static void main(String args[]) throws Exception
	{
		LinkedQueue queue=new LinkedQueue();
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		String []in=br.readLine().split("");
		//ִ����ӣ��ٳ��Ӵ�ӡ
		for(String myin:in)
		{
			queue.add(myin);
		}
		//���ӣ���ӡ
		while(!queue.IsEmpty())
		{
			System.out.print(queue.remove());
		}
	}


}








