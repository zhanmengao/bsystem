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
		//��ȡ,���ýӿ�
		//UpperLetter changeLetter=(UpperLetter) ac.getBean("changeLette");
		//System.out.println(changeLetter.change());
		//ʹ�ýӿ�������bean
		ChangeLetter changeLetter=(ChangeLetter) ac.getBean("changeLette");
		System.out.println(changeLetter.change());
	}

}
