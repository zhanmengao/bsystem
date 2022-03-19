package com.hsp.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.Action;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.hsp.domain.Department;
import com.hsp.domain.Employee;
import com.hsp.service.interfaces.DepartmentServiceInter;
import com.hsp.service.interfaces.EmployeeServiceInter;
import com.hsp.web.forms.EmployeeForm;


public class EmployeeAction extends DispatchAction {

	private DepartmentServiceInter departmentService;
	private EmployeeServiceInter  employeeService;
	
	public void setEmployeeService(EmployeeServiceInter employeeService) {
		this.employeeService = employeeService;
	}
	public void setDepartmentService(DepartmentServiceInter departmentService) {
		this.departmentService = departmentService;
	}
	public ActionForward addEmpUi(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// TODO Auto-generated method stub
		//��ͨ��·
		return mapping.findForward("goAddEmpUi");
	}
	//������ӹ�Ա������
	public ActionForward addEmp(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// TODO Auto-generated method stub
		
		//���������Ƿ��ܹ��õ��û��ύ�ĸ�������
		EmployeeForm employeeForm=(EmployeeForm)form;
		//System.out.println(employeeForm.getDepartmentId()+" "+employeeForm.getEmail());
		
		//����һ��Employee����
		
		Employee employee=new Employee();
		employee.setEmail(employeeForm.getEmail());
		employee.setGrade(Integer.parseInt(employeeForm.getGrade()));
		employee.setHiredate(new java.util.Date());
		employee.setName(employeeForm.getName());
		employee.setPwd(employeeForm.getPwd());
		employee.setSalary(Float.parseFloat(employeeForm.getSalary()));
		
		//�ù�Ա���ĸ�����!
		employee.setDepartment((Department)departmentService.findById(Department.class, Integer.parseInt(employeeForm.getDepartmentId())));
		//����
		try {
			employeeService.add(employee);
		} catch (Exception e) {
			return mapping.findForward("opererr");
		}

		return mapping.findForward("operok");
	}
	
	public ActionForward goListEmp(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		String s_pageNow=request.getParameter("pageNow");
		int pageNow=1;
		if(s_pageNow!=null){
			pageNow=Integer.parseInt(s_pageNow);
		}
		
		// TODO Auto-generated method stub
		request.setAttribute("emplist", employeeService.showEmployList(3,pageNow));
		//����
		int pageCount=employeeService.getPageCount(3);
		request.setAttribute("pageCount", pageCount);
		//��ͨ��·
		return mapping.findForward("goListEmp");
	}
	
	//ɾ����Ա
	public ActionForward delEmp(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		//��ȡ��Ա��id
		String id=request.getParameter("id");
		System.out.println("ɾ����Աid = "+id);
		
		try {
			employeeService.delById(Employee.class, Integer.parseInt(id));
		} catch (Exception e) {
			return mapping.findForward("opererr");
			// TODO: handle exception
		}
		
		//��ͨ��·
		return mapping.findForward("operok");
	}
	
	//��ת����Ա�Ľ���
	public ActionForward goUpdEmpUi(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		//��ȡ��Ա��id
		String id=request.getParameter("id");
		
		//��ȡ��Ա
		Employee e=(Employee) employeeService.findById(Employee.class, Integer.parseInt(id));
	
		//��ʾ��Ա��Ϣ������һ��ҳ��
		request.setAttribute("emp", e);
		
		return mapping.findForward("goUpdEmpUi");
		
	}
	
	//���¹�Ա
	public ActionForward updEmp(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		
		//��ȡ��
		EmployeeForm ef=(EmployeeForm)form;
		
		//ef->doamin
		Employee employee=new Employee();
		employee.setId(Integer.parseInt(ef.getId()));
		employee.setEmail(ef.getEmail());
		employee.setGrade(Integer.parseInt(ef.getGrade()));
		employee.setName(ef.getName());
		employee.setPwd(ef.getPwd());
		employee.setSalary(Float.parseFloat(ef.getSalary()));
		employee.setDepartment((Department)departmentService.findById(Department.class, Integer.parseInt(ef.getDepartmentId())));
		
		try {
			employeeService.update(employee);
		} catch (Exception e) {
			return mapping.findForward("opererr");
		}
		
		return mapping.findForward("operok");
		
	}
	
}
