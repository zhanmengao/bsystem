package com.test;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.service.BybService;
import com.service.UserService;
import com.util.ApplicaionContextUtil;

public class Test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		//������ʹ�ô�ͳ�ķ�����������UserService�� sayHello����
//		UserService userService=new UserService();
//		userService.setName("˳ƽ");
//		userService.sayHello();
		
		//��������ʹ��spring��������������
		//1.�õ�spring ��applicationContext����(��������)
	
		//UserService us=(UserService) ac.getBean("userService");
		//us.sayHello();
		((UserService)ApplicaionContextUtil.getApplicationContext()
		.getBean("userService")).sayHello();
		
		//��ac[����applicatonContext����]��
	//	BybService bybService=(BybService) ac.getBean("bybService");
	//	bybService.sayBye();
		
	}

}
