package com.hsp.service.imp;

import java.io.Serializable;
import java.util.List;

import javax.annotation.Resource;

import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.springframework.transaction.annotation.Transactional;

import com.hsp.basic.BasicService;
import com.hsp.domain.Department;
import com.hsp.domain.Employee;
import com.hsp.service.interfaces.EmployeeServiceInter;

//��������@Transactional�ô�����spring������������ӹܸ� Service������.


public class EmployeeService extends BasicService implements EmployeeServiceInter {


	//��ʾ���й�Ա
	public List showEmployList(int pageSize,int pageNow){
		String hql="from Employee order by id";
		return this.executeQueryByPage(hql, null, pageNow, pageSize);
	}
	
	public int getPageCount(int pageSize){
		
		String hql="select count(*) from Employee";
		
		return this.queryPageCount(hql, null, pageSize);
	}
	
	

	//��֤�û�

	public Employee checkEmploye(Employee e) {
		// TODO Auto-generated method stub
		System.out.println("checkEmploye");
		String hql="from Employee where id=? and pwd=?";
		Object []parameters ={e.getId(),e.getPwd()};
		
		List list=this.executeQuery(hql, parameters);
		if(list.size()==0){
			return null;
		}else{
			return (Employee) list.get(0);
		}
	}

}
