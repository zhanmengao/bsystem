package com.hsp.aop;

import java.lang.reflect.Method;

import org.springframework.aop.MethodBeforeAdvice;

public class MyMethodBeforeAdvice implements MethodBeforeAdvice {

	/**
	 * method: �����÷�������
	 * args: ��method���ݵĲ���
	 * target: Ŀ�����
	 */
	public void before(Method method, Object[] args, Object target)
			throws Throwable {
		// TODO Auto-generated method stub
		System.out.println("��¼��־..."+method.getName());

	}

}
