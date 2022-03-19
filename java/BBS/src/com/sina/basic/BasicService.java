package com.sina.basic;

import java.io.Serializable;
import java.util.List;

import javax.annotation.Resource;

import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.transaction.annotation.Transactional;

@Transactional
public abstract class BasicService implements BasicServiceInter {

	@Resource
	private SessionFactory sessionFactory;
	
	public void setSessionFactory(SessionFactory sessionFactory) {
		this.sessionFactory = sessionFactory;
	}

	public void add(Object obj) {
		
		this.sessionFactory.getCurrentSession().save(obj);

	}

	//统一查询方法(hql)
	public List executeQuery(String hql, Object[] parameters) {
		// TODO Auto-generated method stub
		
		Query query=this.sessionFactory.getCurrentSession().createQuery(hql);

		//注入?值
		if(parameters!=null && parameters.length>0){
			for(int i=0;i<parameters.length;i++){
				query.setParameter(i, parameters[i]);
				
			}
		}
		
		return query.list();
	}

	//分页
	public List executeQueryByPage(String hql, Object[] parameters,
			int pageNow, int PagesSize) {
		// TODO Auto-generated method stub
		Query query=this.sessionFactory.getCurrentSession().createQuery(hql);
		
		if(parameters!=null && parameters.length>0){
			for(int i=0;i<parameters.length;i++){
				query.setParameter(i, parameters[i]);
				
			}
		}
	
		//体现分页
		return query.setFirstResult((pageNow-1)*PagesSize).setMaxResults(PagesSize).list();
	}

	//统一的修改和删除
	public List executeUpdate(String hql, Object[] parameters) {
		// TODO Auto-generated method stub
		return null;
	}

	public Object findById(Class clazz, Serializable id) {
		// TODO Auto-generated method stub
		return this.sessionFactory.getCurrentSession().get(clazz, id);
	}

	public Object uniqueQuery(String hql, Object[] parameters) {
		// TODO Auto-generated method stub
		Query query=this.sessionFactory.getCurrentSession().createQuery(hql);
		//给?赋值
		if(parameters!=null && parameters.length>0){
			for(int i=0;i<parameters.length;i++){
				query.setParameter(i, parameters[i]);
			}
		}
		
		return query.uniqueResult();
	}
	
	public int queryPageCount(String hql, Object[] parameters, int pageSize) {
		// TODO Auto-generated method stub
		//获取rowCount
	/*	List list=this.executeQuery(hql, parameters);
		Iterator iteator=list.iterator();
		if(iteator.hasNext()){
			
		}*/
		Object obj=this.uniqueQuery(hql, parameters);
		//System.out.println("obj value= "+ obj);//obj如果等于rowConunt
		int rowCount=Integer.parseInt(obj.toString());
		
		
		return (rowCount-1)/pageSize+1;
	}
	
	public void delById(Class clazz,Serializable id) {
		// TODO Auto-generated method stub
		Session session=this.sessionFactory.getCurrentSession();
		session.delete(this.findById(clazz, id));
	}
	
	public void update(Object object) {
		// TODO Auto-generated method stub
		this.sessionFactory.getCurrentSession().update(object);
	}
}
