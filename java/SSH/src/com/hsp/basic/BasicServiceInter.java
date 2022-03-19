package com.hsp.basic;

import java.util.List;

public interface BasicServiceInter {

	//声明一些常用的方法
	//1.通过id获取对象
	public Object findById(Class clazz,java.io.Serializable id);
	//2.查询方法 hql 
	public List executeQuery(String hql,Object []parameters);
	//3.查询方法 带分页
	public List executeQueryByPage(String hql,Object []parameters,int pageNow,int PagesSize);
	//4.添加一个对象
	public void add(Object obj);
	//5.统一的执行hql->删除,修改 hql="update domain对象 where ?"
	public List executeUpdate(String hql,Object []parameters);
	//6.返回一个对象的操作
	public Object uniqueQuery(String hql,Object []parameters);
	
	//7.得到hql,返回pageCount
	public int queryPageCount(String hql,Object [] parameters,int pageSize);
	//8.根据id号删除对象
	public void delById(Class clazz,java.io.Serializable id);
	
	//9.通过Object修改
	public void update(Object object);
	
}
