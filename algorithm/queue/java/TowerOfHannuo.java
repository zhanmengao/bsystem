package com.StackAndQueue;
/*
 * ʵ�ֺ�ŵ�����⣺������ʵ�н��������Ĳ��裬�����ⶨ�㷨
 * 
 */

public class TowerOfHannuo {
	public static void main(String args[])
	{
		move(3, 'a', 'c', 'b');
	}
	public static void move(int n,char from,char to,char sup)
	{
		//�˳��ݹ�������n=1�����������Ǹ���ֱ����
		if(n==1)
		{
			System.out.println(from+"->"+to);
		}else{
			move(n-1,from,sup,to);//1����from��n-1�����Ƶ�sup
			System.out.println(from+"->"+to);//2����from�ĵ�n�����Ƶ�to
			move(n-1,sup,to,from);//3����sup�ĵ�n-1�����Ƶ�to
		}
	}

}
