package com.hsp.collection;

import java.util.Enumeration;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;



public class App1 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("com/hsp/collection/beans.xml");
		Department department=(Department) ac.getBean("department");
		System.out.println(department.getName());
		for(String emName:department.getEmpName()){
			System.out.println(emName);
		}
		
		System.out.println("**********通过list集合取出数据*****");
		for(Employee e : department.getEmpList()){
			
			System.out.println("name="+e.getName()+" "+e.getId());
		}
		System.out.println("**********通过set集合取出数据*****");
		for(Employee e : department.getEmpsets()){
			
			System.out.println("name="+e.getName());
		}
		
		System.out.println("*******通过map集合取出数据 迭代器****");
		
		//1.迭代器
		Map<String,Employee> empmaps=department.getEmpMaps();
		Iterator it=empmaps.keySet().iterator();
		while(it.hasNext()){
			String key=(String) it.next();
			Employee emp=empmaps.get(key);
			System.out.println("key="+key+" "+emp.getName());
		}
		
		System.out.println("*******通过map集合取出数据 简洁方法****");
		//2.简洁方法
		for(Entry<String,Employee> entry1:department.getEmpMaps().entrySet()){
			
			System.out.println(entry1.getKey()+" "+entry1.getValue().getName());
		}
		
		System.out.println("*****通过Propertis取出数据*****");
		Properties pp=department.getPp();
		//System.out.println(pp.get("pp1").toString());
		for(Entry<Object,Object> entry:pp.entrySet()){
			System.out.println(entry.getKey().toString()+" "+entry.getValue().toString());
		}
		System.out.println("*****通过Enumeration取出*****");
		Enumeration en= pp.keys();
		while(en.hasMoreElements()){
		//	Entry<Object,Object> elment= (Entry<Object, Object>) en.nextElement();
		//	System.out.println(elment.getKey()+" "+elment.getValue());
			String key=(String) en.nextElement();
			System.out.println(key+" "+pp.getProperty(key));
		}
	}

}
