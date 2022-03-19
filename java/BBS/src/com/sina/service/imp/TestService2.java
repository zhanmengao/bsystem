package com.sina.service.imp;

import javax.annotation.Resource;

import org.hibernate.SessionFactory;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import com.sina.service.inter.TestInter;

//��������@Transactoinal��Ŀ�ľ����������������ȥ�ӹܸ�Service������
@Transactional
public class TestService2 implements TestInter{

	
	private SessionFactory sessionFactory;

	public void setSessionFactory(SessionFactory sessionFactory) {
		this.sessionFactory = sessionFactory;
	}
	
	public void save(Object obj){
		
		this.sessionFactory.getCurrentSession().save(obj);
	}
	//�����ϣ��test2()�����񱻹���
	@Transactional(propagation=Propagation.NOT_SUPPORTED)
	public void test2(){
		
	}
}
