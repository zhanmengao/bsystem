package com.xianxingbiao;
/*
 * ʵ�ֶ�̬����
 */

public class DyyArray {
	//����Ԫ��
	public static void arrar_insert(Object []array,Object value,int position)
	{
		//�쳣���
		if(position>array.length)
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		//���������
		if(array[array.length-1]!=null)
		{
			Object oldarray[]=array;
			//������(����*1.5)
			array=new Object[(int) (oldarray.length*1.5)];
			//���Ƹ�����
			for(int i=0;i<oldarray.length;i++)
			{
				array[i]=oldarray[i];
			}
		}
		//ִ�к���Ԫ��
		for(int i=array.length-2;i>position;i--)
		{
			array[i-1]=array[i-2];
		}
		array[position]=value;
	}
	//�Ƴ�Ԫ��
	public static void array_delete(Object []array,int position)
	{
		//�쳣���
		if(position<0 || position>array.length-1)
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		//ִ��position֮���Ԫ��ǰ��
		array[position-1]=null;
		for(int i=position-1;i<array.length;i++)
		{
			array[i]=array[i+1];
		}
	}

}
