package com.tools;
/*
 * SessionFactory重量级对象的单例类
 * 
 * 在使用hibernate开发项目时，必须保证只有一个SessionFactory
 */

import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;

final public class MySessionFactory {
	private MySessionFactory()
	{
		
	}
	private static SessionFactory sessionFactory=null;
	static {
		sessionFactory=new Configuration().configure().buildSessionFactory();
	}
	
	public static SessionFactory getSessionFactory()
	{
		return sessionFactory;
	}
}
