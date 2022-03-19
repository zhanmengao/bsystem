package com.hsp.service.interfaces;

import java.util.List;

import com.hsp.basic.BasicServiceInter;
import com.hsp.domain.Employee;

public interface EmployeeServiceInter extends BasicServiceInter {

	public List showEmployList(int pageSize,int pageNow);

	//如果该雇员存在，则返回该雇员的完整信息，否则返回null
	public Employee checkEmploye(Employee e);
	
	public int getPageCount(int pageSize);
	
	
}
