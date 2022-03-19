package com.hsp.util;

import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

//在使用hibernate开发项目，请一定保证只有一个SessionFactory
//一个数据库对应一个SessionFactory 对象.
final public class MySessionFactory {

	private static SessionFactory sessionFactory=null;
	
	private MySessionFactory(){
		
	}
	
	static{
		
		sessionFactory =new Configuration().configure("com/hsp/config/hsp.cfg.xml").buildSessionFactory(); 
		System.out.println("sessionFactory 类型"+sessionFactory);
	}
	
	public static SessionFactory getSessionFactory(){
		return sessionFactory;
	}
	
}
