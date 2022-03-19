package com.hsp.beanlife;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

import org.springframework.beans.BeansException;
import org.springframework.beans.factory.BeanFactory;
import org.springframework.beans.factory.BeanFactoryAware;
import org.springframework.beans.factory.BeanNameAware;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.config.BeanPostProcessor;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

public class PersonService implements BeanNameAware,BeanFactoryAware,ApplicationContextAware,InitializingBean {

	private String name;
	private Integer age;

	public Integer getAge() {
		return age;
	}

	public void setAge(Integer age) {
		this.age = age;
	}

	public String getName() {
		return name;
	}
	
	public PersonService(String abc){
		System.out.println("PersonService ����");
	}
	
	public PersonService(){
		System.out.println("PersonService ����");
	}

	public void setName(String name) {
		System.out.println("setName(String name) ����");
		this.name = name;
	}
	
	public void sayHi(){
		System.out.println("hi "+ name);
	}

	//�÷������Ը�arg0��ʾ���ڱ�ʵ������bean id
	public void setBeanName(String arg0) {
		// TODO Auto-generated method stub
		System.out.println("setBeanName ������ ֵ"+arg0);
		
	}

	//�÷������Դ���beanFactroy
	public void setBeanFactory(BeanFactory arg0) throws BeansException {
		// TODO Auto-generated method stub
		System.out.println("setBeanFactory "+arg0);
	}
	//�÷�������ApplicationContext
	public void setApplicationContext(ApplicationContext arg0)
			throws BeansException {
		// TODO Auto-generated method stub
		System.out.println("setApplicationContext"+arg0);
		
	}

	//Ҳ����ͨ��ע��ķ�ʽ�������ĸ�����init-method
	@PostConstruct
	public void init(){
		System.out.println("���Լ���init����");
	}
	
	//	
	public void afterPropertiesSet() throws Exception {
		// TODO Auto-generated method stub
		System.out.println("afterPropertiesSet()");
	}

	//�������ǵ����ٷ���
	@PreDestroy
	public  void mydestory(){
		System.out.println("�ͷŸ�����Դ");
	}


	
}
