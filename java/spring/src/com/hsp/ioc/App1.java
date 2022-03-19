package com.hsp.ioc;

import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.xml.XmlBeanFactory;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.context.support.FileSystemXmlApplicationContext;
import org.springframework.core.io.ClassPathResource;

public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//��ApplicationContext��ȡbean
		//ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/ioc/beans.xml");
		//������ȥʵ����beans.xml,���ļ������õ�bean��ʵ��(��bean scope�� singleton)
		//��bean��ȡ��student
		
		//ͨ���ļ�·������ȡ
		ApplicationContext ac=new FileSystemXmlApplicationContext("src\\com\\hsp\\ioc\\beans.xml");
		
		//��ȡ����student
		Student s1=(Student) ac.getBean("student");
		Student s2=(Student) ac.getBean("student");
		System.out.println(s1+" "+s2);
		
		
		//�������ʹ��beanfactoryȥ��ȡbean������ֻ��ʵ������������ ��ô
		//������bean����ʵ����,ֻ�е���ȥʹ��getBeanĳ��beanʱ���Ż�ʵʱ�Ĵ���.
		
//		BeanFactory factory = new XmlBeanFactory(
//				new ClassPathResource("com/hsp/ioc/beans.xml"));
//		factory.getBean("student");
		
		//�����չ��Ӳ��
	
	}

}
