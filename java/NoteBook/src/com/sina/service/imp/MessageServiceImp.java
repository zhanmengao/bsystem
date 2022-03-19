package com.sina.service.imp;

import java.util.List;

import com.sina.domain.Message;
import com.sina.domain.Users;
import com.sina.service.inter.BaseInterface;
import com.sina.service.inter.MessageServiceInter;
import com.sina.util.HibernateUtil;

public class MessageServiceImp extends BaseServiceImp implements MessageServiceInter {

	public List<Message> showMessage(Users user) {
		// TODO Auto-generated method stub
		String hql="from Message where getter.userid=? or getter.userid=4";
		String parameters[]={user.getUserid()+""};
		List<Message> messages=HibernateUtil.executeQuery(hql, parameters);
		return messages;
	}

	
	
	
}
