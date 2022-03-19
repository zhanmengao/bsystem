package com.sina.test;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.sina.domain.Type;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		ApplicationContext ac=new ClassPathXmlApplicationContext("beans.xml");
		
		//ªÒ»°sessoionFactory
		Session s=((SessionFactory)ac.getBean("sessionFactory")).openSession();
		s.beginTransaction();
		
		Type type=(Type) s.get(Type.class, 1);
		System.out.println(type.getType());
		
		s.beginTransaction().commit();
		s.close();
		System.out.println("**********************");
		Session s2=((SessionFactory)ac.getBean("sessionFactory")).openSession();
		s2.beginTransaction();
		
		Type type2=(Type) s2.get(Type.class, 1);
		System.out.println(type.getType());
		
		s2.beginTransaction().commit();
		s2.close();
	}

}
