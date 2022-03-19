package com.xianxingbiao;
/*
 * 功能：实现线性表顺序存储结构的插入节点、删除节点
 */
public class Array {
	public static void arrar_insert(Object []array,Object value,int position)
	{
		//异常检测
		if(array[array.length-1]!=null || position>array.length)
		{
			return;
		}
		//执行后移元素
		for(int i=array.length-2;i>position;i--)
		{
			array[i-1]=array[i-2];
		}	
	}
	public static void array_delete(Object []array,int position)
	{
		//异常检测
		if(position<0 || position>array.length-1)
		{
			return;
		}
		//执行position之后的元素前移
		array[position-1]=null;
		for(int i=position-1;i<array.length;i++)
		{
			array[i]=array[i+1];
		}
	}
}
