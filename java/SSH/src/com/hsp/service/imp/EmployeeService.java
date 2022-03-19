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

//这里配置@Transactional用处是让spring的事务管理器接管该 Service的事务.


public class EmployeeService extends BasicService implements EmployeeServiceInter {


	//显示所有雇员
	public List showEmployList(int pageSize,int pageNow){
		String hql="from Employee order by id";
		return this.executeQueryByPage(hql, null, pageNow, pageSize);
	}
	
	public int getPageCount(int pageSize){
		
		String hql="select count(*) from Employee";
		
		return this.queryPageCount(hql, null, pageSize);
	}
	
	

	//验证用户

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
