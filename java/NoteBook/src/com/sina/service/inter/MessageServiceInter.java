package com.sina.service.inter;

import java.util.List;

import com.sina.domain.Message;
import com.sina.domain.Users;

public interface MessageServiceInter extends BaseInterface{

	//��ʾ�û����յ�������message
	
	public List<Message> showMessage(Users user);
	
	
}
