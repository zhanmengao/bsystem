package com.sina.service.inter;

import java.util.List;

public interface BaseInterface {
//��һЩͨ�õķ������������û����ӿ�
	//ͨ��id�ţ�����ȡһ������ʵ�� 
	public Object findById(Class clazz,java.io.Serializable id);
	//findById(Users.class,12);//��Ϊ�������еİ�װ�඼ʵ���˸ýӿ�
	//�������
	public void save(Object obj);
	
	//ͨ��hql����ȡList
	public List<Object> getListObject(String hql,String[] parameters); 
	
	//ͨ��hql����ȡһ������.
	public Object getUniqueObject(String hql,String [] parameters);
}
