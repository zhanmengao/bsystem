package com.hsp.test;



import java.util.HashSet;
import java.util.Set;

import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.hsp.domain.Department;
import com.hsp.domain.Employee;
import com.hsp.service.interfaces.DepartmentServiceInter;
import com.hsp.service.interfaces.EmployeeServiceInter;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ApplicationContext ac=new ClassPathXmlApplicationContext("applicationContext.xml");
		DepartmentServiceInter  departmentServiceInter=(DepartmentServiceInter) ac.getBean("departmentService");
		EmployeeServiceInter  employeeServiceInter=(EmployeeServiceInter) ac.getBean("employeeService");
		
		Employee employee=new Employee("韩顺平1","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		Employee employee2=new Employee("韩顺平3","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		Employee employee3=new Employee("韩顺平5","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		
		
	//	employeeServiceInter.addEmployee(employee);
	//	employeeServiceInter.addEmployee(employee2);
	//	employeeServiceInter.addEmployee(employee3);
		
		Department d1=new Department("财务部");
		
	
		
	
		
		//添加部门
	//	departmentServiceInter.addDepartment(d1);
		
		
		
		
		
		
		
		
//		Employee e=employeeServiceInter.getEmployeeById(2);
//		Employee e2=employeeServiceInter.getEmployeeById(2);
//		Employee e3=employeeServiceInter.getEmployeeById(4);
//		Employee e4=employeeServiceInter.getEmployeeById(2);
//		
		//证明是从二级缓存，而不是从一级缓存取出的?
		//取出SessionFactory
		//1.通过employeeServiceInter
		//2.直接从spring容器取出，主要目的是测试
//		SessionFactory sf=(SessionFactory) ac.getBean("sessionFactory");
//		//second level cache puts=2,second level cache hits=2,second level cache misses=2,
//		System.out.println(sf.getStatistics());
//		
//		System.out.println(e.getName());
		
	
		
	}

}
