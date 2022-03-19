package com.hsp.web.action;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.hsp.domain.Employee;
import com.hsp.service.interfaces.EmployeeServiceInter;
import com.hsp.web.forms.EmployeeForm;

public class LoginAction extends DispatchAction {
	
	@Resource
	private EmployeeServiceInter  employeeService;
	private int a=0;
	
	//��Ӧ��¼
	public ActionForward login(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		System.out.println("******ͨ���µķ�ʽ��Ӧ����***********");
		System.out.println("a="+(++a));
	
		// TODO Auto-generated method stub
		//ȡ�����������ȴ�ͨ��ϰ,���Ǽ���֤
		EmployeeForm employeeForm=(EmployeeForm)form;
		//����һ��Employee����
		Employee e=new Employee();
		e.setId(Integer.parseInt(employeeForm.getId()));
		e.setPwd(employeeForm.getPwd());
		e=employeeService.checkEmploye(e);
		if(e!=null){
			//�ѹ�Ա��Ϣ����session,�������ʹ��
			request.getSession().setAttribute("loginuser", e);
			return mapping.findForward("ok");
		}else{
			return mapping.findForward("err");
		}
		
	}
	
	//��Ӧ��¼
	public ActionForward goMainFrame(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		//û�ж������֤![������]
		
		if(request.getSession().getAttribute("loginuser")!=null){
		
			return mapping.findForward("ok");
		}else{
			return mapping.findForward("err");
		}
		
	}
	
	//��Ӧע������
	public ActionForward logout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// TODO Auto-generated method stub
		return super.execute(mapping, form, request, response);
	}

	
	public void setEmployeeService(EmployeeServiceInter employeeService) {
		System.out.println("setEmployeeServiceInter ������");
		this.employeeService= employeeService;
	}
}
