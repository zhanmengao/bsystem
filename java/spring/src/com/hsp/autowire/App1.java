package com.hsp.autowire;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/autowire/beans.xml");
		//»ñÈ¡
		Master master=(Master) ac.getBean("master");
		System.out.println(master.getName()+" Ñø "+master.getDog().getName());
	}

}
