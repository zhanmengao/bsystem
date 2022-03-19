package com.hsp.ioc;


//java类都有一个默认的无参构造函数
public class Student {

	String name;

	public Student(){
		System.out.println("student被创建");
	}
	
	public String getName() {
		return name;
	}

	public void setName(String name) {
		//this.name = name;
		System.out.println("输入值"+name);
	}
	
	
}
