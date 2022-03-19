package com.hsp.beanlife;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.xml.XmlBeanFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.core.io.ClassPathResource;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	//	ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/beanlife/beans.xml");
		
		BeanFactory factory = new XmlBeanFactory(
				new ClassPathResource("com/hsp/beanlife/beans.xml"));
		
		PersonService ps=(PersonService) factory.getBean("personService");
		ps.sayHi();
		
		
	}

}
