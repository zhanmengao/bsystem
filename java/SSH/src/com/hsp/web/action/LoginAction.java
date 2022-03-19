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
	
	//响应登录
	public ActionForward login(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		System.out.println("******通过新的方式响应请求***********");
		System.out.println("a="+(++a));
	
		// TODO Auto-generated method stub
		//取出表单，我们先打通练习,我们简单验证
		EmployeeForm employeeForm=(EmployeeForm)form;
		//构建一个Employee对象
		Employee e=new Employee();
		e.setId(Integer.parseInt(employeeForm.getId()));
		e.setPwd(employeeForm.getPwd());
		e=employeeService.checkEmploye(e);
		if(e!=null){
			//把雇员信息放入session,后面可以使用
			request.getSession().setAttribute("loginuser", e);
			return mapping.findForward("ok");
		}else{
			return mapping.findForward("err");
		}
		
	}
	
	//响应登录
	public ActionForward goMainFrame(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		//没有对身份验证![过滤器]
		
		if(request.getSession().getAttribute("loginuser")!=null){
		
			return mapping.findForward("ok");
		}else{
			return mapping.findForward("err");
		}
		
	}
	
	//响应注销请求
	public ActionForward logout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// TODO Auto-generated method stub
		return super.execute(mapping, form, request, response);
	}

	
	public void setEmployeeService(EmployeeServiceInter employeeService) {
		System.out.println("setEmployeeServiceInter 被调用");
		this.employeeService= employeeService;
	}
}
