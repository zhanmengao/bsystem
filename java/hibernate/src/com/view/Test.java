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
		//模板
		Session session=HibernateUtil.getCurrentSession();
		Transaction ts=null;
		
		try {
			ts=session.beginTransaction();
			//获取query引用[这里Employee不是表，而是domain对象]
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
		//删除
		Session session=MySessionFactory.getSessionFactory().openSession();
		Transaction transaction=session.beginTransaction();
		//获取该雇员，然后删除
		Employee emp=(Employee) session.load(Employee.class, 1);
		session.delete(emp);
		transaction.commit();
		session.close();
	}

	public static void UpdateEmp() {
		//模板
		Session session=MySessionFactory.getSessionFactory().openSession();
		Transaction ts=null;
		
		try {
			ts=session.beginTransaction();
			Employee emp=(Employee) session.load(Employee.class, 2);
			emp.setMyname("新的111");
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
		//这里我们只建对象
		//1.创建Configuration
		//该对象用于读取hibernate.cfg.xml,并完成初始化
		Configuration configuratio=new Configuration().configure();
		//2、创建SessoinFactory，这是一个会话工厂，是一个重量级对象
		SessionFactory sessionFactory=configuratio.buildSessionFactory();
		//3.创建一个会话 相当于jdbc中的Connection
		Session session=sessionFactory.openSession();
		//4.对hibernate而言，进行DDL语句要求程序员使用事务。
		Transaction transaction=session.beginTransaction();
		//添加一个雇员
		Employee newemp=new Employee();
		newemp.setMyname("biyu");
		newemp.setEmail("aaaaa@163.com");
		newemp.setHiredate(new Date());
		//保存
		session.save(newemp);//-->insert into语句 被hibernate封装
		//提交事务
		transaction.commit();
		//关闭资源
		session.close();
	}

}
