package com.hsp.service.interfaces;

import java.util.List;

import com.hsp.basic.BasicServiceInter;
import com.hsp.domain.Employee;

public interface EmployeeServiceInter extends BasicServiceInter {

	public List showEmployList(int pageSize,int pageNow);

	//����ù�Ա���ڣ��򷵻ظù�Ա��������Ϣ�����򷵻�null
	public Employee checkEmploye(Employee e);
	
	public int getPageCount(int pageSize);
	
	
}
