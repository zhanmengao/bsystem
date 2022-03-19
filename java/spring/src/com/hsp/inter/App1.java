package com.hsp.inter;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/inter/beans.xml");
		//获取,不用接口
		//UpperLetter changeLetter=(UpperLetter) ac.getBean("changeLette");
		//System.out.println(changeLetter.change());
		//使用接口来访问bean
		ChangeLetter changeLetter=(ChangeLetter) ac.getBean("changeLette");
		System.out.println(changeLetter.change());
	}

}
