package com.xianxingbiao;
/*
 * ���ܣ�ʵ�����Ա�˳��洢�ṹ�Ĳ���ڵ㡢ɾ���ڵ�
 */
public class Array {
	public static void arrar_insert(Object []array,Object value,int position)
	{
		//�쳣���
		if(array[array.length-1]!=null || position>array.length)
		{
			return;
		}
		//ִ�к���Ԫ��
		for(int i=array.length-2;i>position;i--)
		{
			array[i-1]=array[i-2];
		}	
	}
	public static void array_delete(Object []array,int position)
	{
		//�쳣���
		if(position<0 || position>array.length-1)
		{
			return;
		}
		//ִ��position֮���Ԫ��ǰ��
		array[position-1]=null;
		for(int i=position-1;i<array.length;i++)
		{
			array[i]=array[i+1];
		}
	}
}
