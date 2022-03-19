package com.sina.service.inter;

import java.util.List;

public interface BaseInterface {
//把一些通用的方法，声明到该基础接口
	//通过id号，来获取一个对象实例 
	public Object findById(Class clazz,java.io.Serializable id);
	//findById(Users.class,12);//因为我们所有的包装类都实现了该接口
	//保存对象
	public void save(Object obj);
	
	//通过hql语句获取List
	public List<Object> getListObject(String hql,String[] parameters); 
	
	//通过hql语句获取一个对象.
	public Object getUniqueObject(String hql,String [] parameters);
}
