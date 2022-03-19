package com.view;
import java.util.Date;
import java.util.List;

import org.hibernate.Query;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import org.hibernate.cfg.*;

import com.domain.Employee;
import com.sun.scenario.effect.impl.sw.sse.SSEBlend_ADDPeer;
import com.tools.HibernateUtil;
import com.tools.MySessionFactory;

public class Test {
	

	public static void main(String[] args) {
		
		
		
	}

	public static void Query() {
		//ģ��
		Session session=HibernateUtil.getCurrentSession();
		Transaction ts=null;
		
		try {
			ts=session.beginTransaction();
			//��ȡquery����[����Employee���Ǳ�����domain����]
			Query query=session.createQuery("from  Employee where myid='1'");
			List<Employee> list=query.list();
			for(Employee emp:list)
			{
				System.out.println(emp.getMyid()+" "+emp.getMyname());
			}
		}finally{
			if(session!=null && session.isOpen())
				session.close();
		}
	}

	public static void DeleteEmp() {
		//ɾ��
		Session session=MySessionFactory.getSessionFactory().openSession();
		Transaction transaction=session.beginTransaction();
		//��ȡ�ù�Ա��Ȼ��ɾ��
		Employee emp=(Employee) session.load(Employee.class, 1);
		session.delete(emp);
		transaction.commit();
		session.close();
	}

	public static void UpdateEmp() {
		//ģ��
		Session session=MySessionFactory.getSessionFactory().openSession();
		Transaction ts=null;
		
		try {
			ts=session.beginTransaction();
			Employee emp=(Employee) session.load(Employee.class, 2);
			emp.setMyname("�µ�111");
			emp.setEmail("23123@qq.com");
			ts.commit();
		} catch (Exception e) {
			if(ts!=null)
				ts.rollback();
			throw new RuntimeException(e.getMessage());
		}finally{
			if(session!=null && session.isOpen())
				session.close();
		}
	}

	public static void InsertEmp() {
		//��������ֻ������
		//1.����Configuration
		//�ö������ڶ�ȡhibernate.cfg.xml,����ɳ�ʼ��
		Configuration configuratio=new Configuration().configure();
		//2������SessoinFactory������һ���Ự��������һ������������
		SessionFactory sessionFactory=configuratio.buildSessionFactory();
		//3.����һ���Ự �൱��jdbc�е�Connection
		Session session=sessionFactory.openSession();
		//4.��hibernate���ԣ�����DDL���Ҫ�����Աʹ������
		Transaction transaction=session.beginTransaction();
		//���һ����Ա
		Employee newemp=new Employee();
		newemp.setMyname("biyu");
		newemp.setEmail("aaaaa@163.com");
		newemp.setHiredate(new Date());
		//����
		session.save(newemp);//-->insert into��� ��hibernate��װ
		//�ύ����
		transaction.commit();
		//�ر���Դ
		session.close();
	}

}
