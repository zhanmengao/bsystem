package com.sina.service.imp;

import java.util.List;

import com.sina.domain.Users;
import com.sina.service.inter.UsersServiceInter;
import com.sina.util.HibernateUtil;
import com.sina.util.MyTools;

//���һ��ҵ�����࣬��ɶ�users domian����ĸ��ֲ���.
public class UsersServiceImp extends BaseServiceImp implements UsersServiceInter{

	//��֤�û��Ƿ�Ϸ�
	/**
	 * @author ˳ƽ
	 * @function: ����û���֤
	 * @����˵�� 
	 * @return �����֤�Ϸ�������������user��Ϣ,���ʧ�ܷ���null
	 */
	public Users checkUser(Users user){
		
		String hql="from Users where userid=? and userpwd=?";
		String parameters[]={user.getUserid()+"",MyTools.MD5(user.getUserpwd())};
		//��������hibernate��³����
		List<Users> list=HibernateUtil.executeQuery(hql, parameters);
		if(list.size()==0){
			return null;
		}else{
			return list.get(0);
		}
		
	}

	public Users getUserByName(String name) {
		// TODO Auto-generated method stub
		String hql="from Users where name=?";
		String parameters[]={name};
		Users user=null;
		try {
			user=(Users) HibernateUtil.uniqueQuery(hql, parameters);
		} catch (Exception e) {
			throw new RuntimeException(e.getMessage());
			// TODO: handle exception
		}
		return user;
		
	}
	
	
}
