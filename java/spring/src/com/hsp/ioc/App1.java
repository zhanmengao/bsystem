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
		//从ApplicationContext中取bean
		//ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/ioc/beans.xml");
		//当我们去实例化beans.xml,该文件中配置的bean被实例(该bean scope是 singleton)
		//从bean中取出student
		
		//通过文件路径来获取
		ApplicationContext ac=new FileSystemXmlApplicationContext("src\\com\\hsp\\ioc\\beans.xml");
		
		//获取两个student
		Student s1=(Student) ac.getBean("student");
		Student s2=(Student) ac.getBean("student");
		System.out.println(s1+" "+s2);
		
		
		//如果我们使用beanfactory去获取bean，当你只是实例化该容器， 那么
		//容器的bean不被实例化,只有当你去使用getBean某个bean时，才会实时的创建.
		
//		BeanFactory factory = new XmlBeanFactory(
//				new ClassPathResource("com/hsp/ioc/beans.xml"));
//		factory.getBean("student");
		
		//软件发展《硬件
	
	}

}
