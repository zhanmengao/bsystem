package com.hsp.dispatch;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/dispatch/beans.xml");
	
		DBUtil dbUtil=(DBUtil) ac.getBean("dbutil2");
		System.out.println(dbUtil.getDrivername()+" "+dbUtil.getName());
	
	}

}
