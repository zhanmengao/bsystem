package com.StackAndQueue;
import java.util.*;

public class TwoStackToQueue<T> {
	private Stack<T> stack1;
	private Stack<T> stack2;
	TwoStackToQueue()
	{
		stack1=new Stack<T>();
		stack2=new Stack<T>();
	}
	public void Push(T obj)
	{
		//���뵽ջ1
		stack1.push(obj);
	}
	public T Pop()
	{
		if(!stack2.isEmpty())
		{
			//�����Ϊ�գ�ֱ�ӷ���ջ2��Ԫ��
			return stack2.pop();
		}else{
			//��֯ջ1��Ԫ�ؽ���ջ2
			while(!stack1.isEmpty())
			{
				stack2.push(stack1.pop());
			}
			if(stack2.isEmpty())
			{
				throw new RuntimeException("�ն��У�");
			}
			else{
				//�Ѷ���2��Ԫ�س�ջ
				return stack2.pop();
			}
		}
	}
}











