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
		
		Employee employee=new Employee("��˳ƽ1","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		Employee employee2=new Employee("��˳ƽ3","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		Employee employee3=new Employee("��˳ƽ5","aa@sohu.com","145",1,new java.util.Date(),234.56f);
		
		
	//	employeeServiceInter.addEmployee(employee);
	//	employeeServiceInter.addEmployee(employee2);
	//	employeeServiceInter.addEmployee(employee3);
		
		Department d1=new Department("����");
		
	
		
	
		
		//��Ӳ���
	//	departmentServiceInter.addDepartment(d1);
		
		
		
		
		
		
		
		
//		Employee e=employeeServiceInter.getEmployeeById(2);
//		Employee e2=employeeServiceInter.getEmployeeById(2);
//		Employee e3=employeeServiceInter.getEmployeeById(4);
//		Employee e4=employeeServiceInter.getEmployeeById(2);
//		
		//֤���ǴӶ������棬�����Ǵ�һ������ȡ����?
		//ȡ��SessionFactory
		//1.ͨ��employeeServiceInter
		//2.ֱ�Ӵ�spring����ȡ������ҪĿ���ǲ���
//		SessionFactory sf=(SessionFactory) ac.getBean("sessionFactory");
//		//second level cache puts=2,second level cache hits=2,second level cache misses=2,
//		System.out.println(sf.getStatistics());
//		
//		System.out.println(e.getName());
		
	
		
	}

}
