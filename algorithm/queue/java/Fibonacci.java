package com.StackAndQueue;

public class Fibonacci {
	//��쳲��������е�n������ֵ--�Ե����ϵĶ�̬�滮
	//����:n
	//�����Fibonacci[n-1]--��0��ʼ���㣬��ʱn-1��쳲��������е�n������ֵ
	public int FromFloor(int n)
	{
		int[] Fibonacci =new int[n];
		Fibonacci[0]=0;
		Fibonacci[1]=1;
		for(int i=2;i<n;i++)
		{
			Fibonacci[i]=Fibonacci[i-2]+Fibonacci[i-1];
		}
		return Fibonacci[n-1];
	}
	//��쳲��������е�n������ֵ--�Զ����µĶ�̬�滮
	//����:n
	//�����Fibonacci[n-1]--��0��ʼ���㣬��ʱn-1��쳲��������е�n������ֵ
	public int FromTop(int n)
	{
		int[] Fibonacci =new int[n];
		Fibonacci[0]=0;
		Fibonacci[1]=1;
		//������ضϵݹ飺���֮ǰ�Ѿ�������ˣ��ʹӸ�������ȡ
		if(Fibonacci[n-1]!=0)
			return Fibonacci[n-1];
		//��һ��ݹ�ʽ�Ĳ�ͬ������������������ݹ�ʽ����������ȡ��Ԫ��
		return Fibonacci[n-1]=FromTop(n-1)+FromTop(n-2);
	}
	
	public static void main(String args[])
	{

	}
	
	
	
	
	
}
