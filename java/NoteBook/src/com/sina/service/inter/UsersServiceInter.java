package com.sina.service.inter;

import com.sina.domain.Users;

public interface UsersServiceInter extends BaseInterface{

	
	//这里定义方法
	public Users checkUser(Users user);
	//定义通过用户名来获取User
	public Users getUserByName(String name);
}
