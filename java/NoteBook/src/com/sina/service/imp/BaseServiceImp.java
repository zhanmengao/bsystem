package com.sina.service.imp;

import java.io.Serializable;
import java.util.List;

import com.sina.service.inter.BaseInterface;
import com.sina.util.HibernateUtil;

public abstract  class BaseServiceImp implements BaseInterface {

	public Object findById(Class clazz, Serializable id) {
		// TODO Auto-generated method stub
		return HibernateUtil.findById(clazz, id);
	}
	//¸ù¾ÝidÉ¾³ý...
	public void save(Object obj){
		
		try {
			HibernateUtil.save(obj);
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
			// TODO: handle exception
		}
	}
	public List<Object> getListObject(String hql, String[] parameters) {
		// TODO Auto-generated method stub
		return null;
	}
	public Object getUniqueObject(String hql, String[] parameters) {
		// TODO Auto-generated method stub
		return null;
	}
	
	

}
