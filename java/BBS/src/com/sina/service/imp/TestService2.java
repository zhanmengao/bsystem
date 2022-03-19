package com.sina.service.imp;

import javax.annotation.Resource;

import org.hibernate.SessionFactory;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import com.sina.service.inter.TestInter;

//这里配置@Transactoinal的目的就是让事务管理器，去接管该Service的事务
@Transactional
public class TestService2 implements TestInter{

	
	private SessionFactory sessionFactory;

	public void setSessionFactory(SessionFactory sessionFactory) {
		this.sessionFactory = sessionFactory;
	}
	
	public void save(Object obj){
		
		this.sessionFactory.getCurrentSession().save(obj);
	}
	//如果你希望test2()的事务被管理
	@Transactional(propagation=Propagation.NOT_SUPPORTED)
	public void test2(){
		
	}
}
