package com.StackAndQueue;
/*
 * ��������ѭ������
 */

public class DyyLoopQueue {
	private Object array[];
	private int head;//ָ����һ��Ҫ���ӵ���
	private int reaf;//ָ����һ�����Բ���Ԫ�صĿռ�
	private int size;//���е�Ԫ�ظ���
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
	//����в���
	//���룺Ҫ������е�Ԫ��
	public void push(Object value)
	{
		//���βָ������Ҫ׷��ͷָ��->����
		if((reaf+1)%array.length==head)
		{
			int size=array.length;
			//����
			Object[] newarray=new Object[size*2];
			System.arraycopy(array, 0, newarray, 0, size);
			size=size*2;
			array=newarray;
		}
		array[reaf]=value;
		//����reaf
		reaf=(reaf+1)%(array.length);
		size++;
	}
	//�����в���
	//���������Ԫ��
	public Object pop()
	{
		if(IsEmpty())
		{
			throw new RuntimeException();
		}
		Object nowtop=array[head];
		//topָ�����һλ 
		head=(head+1)%array.length;
		size--;
		
		return nowtop;
	}
	public int getSize() {
		return size;
	}
	//��������뵽ԭ�ȶ���
	//���룺�½ڵ�ֵ
	public void Insert_ASC(Object data)
	{
		//�ҵ�����λ��  
	}
}











