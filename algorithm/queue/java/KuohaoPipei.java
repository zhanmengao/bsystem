package com.StackAndQueue;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class KuohaoPipei {
	//�ж������Ƿ�ƥ��
	//���룺Java���
	//������Ƿ� error
	public boolean Pipei(String str)
	{
		boolean boo=true;
		LinkedStack linkedStack=new LinkedStack();
		char in[]=str.toCharArray();
		//ѭ��ȡ��
		for(char c:in)
		{
			//����������ţ���ջ
			if('('==c || '{'==c || '['==c)
			{
				//��ջ
				linkedStack.push(c);
			}
			else if(')'==c)
			{
				//���ջ����ƥ�䲻����Ӧ��������->����
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//������ǰһ�����������ţ��������->����
				if(a!='(')
				{
					boo=false;
					break;
				}	
			}
			else if('}'==c)
			{
				//���ջ����ƥ�䲻����Ӧ��������->����
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//������ǰһ�����������ţ��������->����
				if(a!='}')
				{
					boo=false;
					break;
				}	
			}else if('['==c)
			{
				//���ջ����ƥ�䲻����Ӧ��������->����
				if(linkedStack.IsEmpty())
				{
					boo=false;
					break;	
				}
				Character a=(Character) linkedStack.pop();
				//������ǰһ�����������ţ��������->����
				if(a!=']')
				{
					boo=false;
					break;
				}	
			}
		}
		//�������ʱ��ջ���滹�����ţ������
		if(!linkedStack.IsEmpty())
			boo=false;
		
		return boo;
	}
	public static void main(String args[])
	{
		KuohaoPipei kuohaoPipei=new KuohaoPipei();
		System.out.println(kuohaoPipei.Pipei("aaa"));
	}
}
