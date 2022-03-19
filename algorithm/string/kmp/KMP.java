package com.lang.String;
/*
 * ģʽ��������
 * Ŀ�꣺�Ӵ�
 */

public class KMP {
	public static int index_KMP(String primary,String sub,int begin)
	{
		int next[]=getNext(sub);
		//��BF�㷨���иĽ�
		char char_pri[]=primary.substring(begin).toCharArray();//����
		char char_sub[]=sub.toCharArray();//ģʽ��
		int i=0,j=0;
		while(i<char_pri.length && j<char_sub.length)
		{
			//�����ǰԪ����ȣ������
			if(char_pri[i]==char_sub[j])
			{
				i++;
				j++;
				//�����ʱƥ�䵽β��
				if(j==sub.length())
				{
					return i-j;
				}
			}
			else if(j>0){
				//����������Ӵ���������ȣ����next[j-1]��ʼ����ƥ��
				j=next[j-1];//next[j-1]����ʾjǰ����next[j-1]�����������->��ô���Ǿʹӵ�һ����i��ȵ�j��ʼƥ��
			}else{
				//�����һ���Ͳ���ȣ�������ָ��iֱ�Ӻ���
				i++;
			}
			
		}
		return -1;
		
	}
	//���ܣ�����KMP��next[]����
	private static int [] getNext(String sub)
	{
		char in[]=sub.toCharArray();
		int []next=new int[sub.length()];//��һ����ԭ�ַ���������ȵ�next����->ָʾ�ַ�����һ�ε�ƥ��
		next[0]=0;
		int top=1;//��׺�±�
		int bottom=0;//ǰ׺�±�
		while(top<next.length)
		{
			if(in[top]==in[bottom])
			{
				next[top]=bottom+1;//next[j]=ģʽ��ǰ��׺��ȵ�Ԫ�ظ���n+�Լ�Ҳ���
				//����
				top++;
				bottom++;		
			}else if(bottom>0){
				//��˷����ǰ�ַ���ǰ׺���׺������������ڴ˴�����P��T�Ĳ�ƥ�䣬���ҵ�next[j-1]��λ�ÿ�ʼ��һ��ƥ��
				bottom=next[bottom-1];
				
				//����ѭ��i���䣬j���˷��֮ǰ������λ�ã��뵱ǰin[i]�����Ƚ�
			}else{
				//�����ʱ�ڸ�char[0]�Ƚ��Ҳ����->next[i]=0
				next[bottom]=0;
				top++;
			}
		}
		return next;
	}
	public static void main(String args[])
	{
		System.out.println(index_KMP("������ëè���ֳ����޶�ë����è���������˰�ŷ��è����������è���Ը��������ɣ��Ǽ�è�еĴ�ͳƷ�֡�1971�꣬�䱻ѡΪ������õ�è��֮һ","ŷ��è����������è",0));
	}
}
