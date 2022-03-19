package com.StackAndQueue;
/*
 * ���õ�����ʵ��ջ
 * head->top
 */
import com.xianxingbiao.*;

public class LinkedStack <T>{
	
	private LinkList<T> list;
	//����һ����ջ
	public LinkedStack()
	{
		list=new LinkList<T>();
	}
	//��ջ
	//���룺�½ڵ��valueֵ
	public void push(T value)
	{
		list.add(value, list.size());
	}
	//��ջ
	//�����ջ��Ԫ��
	public T pop()
	{
		return list.remove(list.size());
	}
	//�ж�ջ��
	public boolean IsEmpty()
	{
		return (list.size()==0);
	}
}






