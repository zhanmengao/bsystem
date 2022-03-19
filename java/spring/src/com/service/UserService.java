package com.service;

public class UserService {

	private String name;
	
	private BybService byeService;

	public BybService getByeService() {
		return byeService;
	}

	public void setByeService(BybService byeService) {
		this.byeService = byeService;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public void sayHello(){
		System.out.println("hello "+name );
		byeService.sayBye();
	}
	
}
