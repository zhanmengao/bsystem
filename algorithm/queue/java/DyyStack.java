package com.StackAndQueue;

public class DyyStack{
	private int size;//����
	private int top;//ͷ������λ��
	private Object array[];//����
	//��ʼ��
	public DyyStack()
	{
		this.size=size;
		array= new Object[5];
		top=-1;
	}
	//���ܣ��ж�ջ��  O(1)
	private boolean IsFull()
	{
		if(this.top==this.size-1)
		{
			return true;
		}else{
			return false;
		}
	}
	//���ܣ��ж�ջ�� O(1)
	public boolean IsEmpty()
	{
		return (this.top==-1);
	}
	
	//���ܣ���ջ O(1)
	//���룺�µ�ջ��
	public void Push(Object newnode)
	{
		if(this.IsFull())
		{
			//����
			Object[] newarray=new Object[size*2];
			System.arraycopy(array, 0, newarray, 0, size);
			size=size*2;
			array=newarray;
		}
		array[++top]=newnode;
	}
	//���ܣ���ջ O(1)
	//�����ջ��Ԫ��
	public Object pop()
	{
		if(this.IsEmpty())
		{
			throw new RuntimeException();	
		}
		return array[top--];
	}
	//���ܣ�����ջ O(1)
	public void ClearStack(int newsize)
	{
		array=new Object[newsize];
	}
	//���ܣ�����ջ O(1)
	public void MakeNullStack()
	{
		array=null;
	}
	//���ܣ�����ջ��ʣ������ O(1)
	public int getNowSize()
	{
		return this.size-this.top;
	}
}
