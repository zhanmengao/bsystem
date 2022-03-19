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

		//我们先使用传统的方法，来调用UserService的 sayHello方法
//		UserService userService=new UserService();
//		userService.setName("顺平");
//		userService.sayHello();
		
		//我们现在使用spring来完成上面的任务
		//1.得到spring 的applicationContext对象(容器对象)
	
		//UserService us=(UserService) ac.getBean("userService");
		//us.sayHello();
		((UserService)ApplicaionContextUtil.getApplicationContext()
		.getBean("userService")).sayHello();
		
		//从ac[代表applicatonContext容器]中
	//	BybService bybService=(BybService) ac.getBean("bybService");
	//	bybService.sayBye();
		
	}

}
