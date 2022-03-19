package com.xianxingbiao;
/*
 * 实现动态数组
 */

public class DyyArray {
	//插入元素
	public static void arrar_insert(Object []array,Object value,int position)
	{
		//异常检测
		if(position>array.length)
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		//数组满检测
		if(array[array.length-1]!=null)
		{
			Object oldarray[]=array;
			//自增长(长度*1.5)
			array=new Object[(int) (oldarray.length*1.5)];
			//复制该数组
			for(int i=0;i<oldarray.length;i++)
			{
				array[i]=oldarray[i];
			}
		}
		//执行后移元素
		for(int i=array.length-2;i>position;i--)
		{
			array[i-1]=array[i-2];
		}
		array[position]=value;
	}
	//移除元素
	public static void array_delete(Object []array,int position)
	{
		//异常检测
		if(position<0 || position>array.length-1)
		{
			throw new ArrayIndexOutOfBoundsException();
		}
		//执行position之后的元素前移
		array[position-1]=null;
		for(int i=position-1;i<array.length;i++)
		{
			array[i]=array[i+1];
		}
	}

}
