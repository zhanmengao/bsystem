package com.sina.service.imp;

import java.util.List;

import com.sina.domain.Users;
import com.sina.service.inter.UsersServiceInter;
import com.sina.util.HibernateUtil;
import com.sina.util.MyTools;

//这个一个业务层的类，完成对users domian对象的各种操作.
public class UsersServiceImp extends BaseServiceImp implements UsersServiceInter{

	//验证用户是否合法
	/**
	 * @author 顺平
	 * @function: 完成用户验证
	 * @参数说明 
	 * @return 如果验证合法，返回完整的user信息,如果失败返回null
	 */
	public Users checkUser(Users user){
		
		String hql="from Users where userid=? and userpwd=?";
		String parameters[]={user.getUserid()+"",MyTools.MD5(user.getUserpwd())};
		//这里体现hibernate的鲁棒性
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
