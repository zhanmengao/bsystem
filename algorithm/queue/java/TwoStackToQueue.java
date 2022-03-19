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
		//插入到栈1
		stack1.push(obj);
	}
	public T Pop()
	{
		if(!stack2.isEmpty())
		{
			//如果不为空，直接返回栈2首元素
			return stack2.pop();
		}else{
			//组织栈1的元素进入栈2
			while(!stack1.isEmpty())
			{
				stack2.push(stack1.pop());
			}
			if(stack2.isEmpty())
			{
				throw new RuntimeException("空队列！");
			}
			else{
				//把队列2首元素出栈
				return stack2.pop();
			}
		}
	}
}











