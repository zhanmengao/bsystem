package com.hsp.aop;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/aop/beans.xml");
		TestServiceInter ts=(TestServiceInter) ac.getBean("proxyFactoryBean");
		ts.sayHello();
		((TestServiceInter2)ts).sayBye();
		
	}

}
