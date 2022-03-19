package com.hsp.constructor;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/constructor/beans.xml");
	
		Employee ee=(Employee) ac.getBean("employee");
		System.out.println(ee.getName());
	
	}

}
