/*
 * 3����д���������ֱ�������n!.һ��ʹ�õݹ鷽��ʵ�֣���һ�����õݹ鷽����
 */
package com.pra;

import java.util.Scanner;

public class Demo3_3 {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
	}
	//�ݹ�ʵ�ּ������n! ���룺����Ԫ��n �����n!
	public int recursion(int n)
	{
		//�����ǰ�ǻ������Σ�����1
		if(n==0||n==1)
		{
			return 1;
		}
		//�����ǰ����Ҫ�����㣬������������n*(n-1)*(n-2)*����)��ֱ����������
		else 
		{
			return n*recursion(n-1);
		}
		
	}
	//ѭ�������۳�
	public int loop()
	{
		Scanner sr=new Scanner(System.in);
		//�ӿ���̨��ȡn
		int n=sr.nextInt();
		int accumulate=1;
		//ֻҪn����0�ͼ�������
		while(n>0)
		{
			accumulate=accumulate*n;
			n--;
		}
		return accumulate;
	}
}
