package com.sina.service.inter;

import com.sina.domain.Users;

public interface UsersServiceInter extends BaseInterface{

	
	//���ﶨ�巽��
	public Users checkUser(Users user);
	//����ͨ���û�������ȡUser
	public Users getUserByName(String name);
}
