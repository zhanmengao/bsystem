package com.hsp.view;

import java.util.HashSet;
import java.util.Set;

import org.hibernate.Hibernate;
import org.hibernate.Session;
import org.hibernate.Transaction;

import com.hsp.domain.Department;
import com.hsp.domain.Student;
import com.hsp.util.HibernateUtil;

public class TestMain {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		// TODO Auto-generated method stub
		//通过获取一个sesion,让hibernate框架运行(config->加载hibernate.cfg.xml)
		Session s=null;
		Transaction tx=null;
		
		try {
			//我们使用基础模板来讲解.
			s=HibernateUtil.openSession();
			tx=s.beginTransaction();
			
			//查询45号学生
		
			Student stu1=(Student) s.get(Student.class, 45);//45->一级缓存		
				tx.commit();
			
		} catch (Exception e) {
			e.printStackTrace();
			if(tx!=null){
				tx.rollback();
			}
		}finally{
			
			if(s!=null && s.isOpen()){
				s.close();
			}
		}
		
		System.out.println("*********************************");
		try {
			//我们使用基础模板来讲解.
			s=HibernateUtil.openSession();
			tx=s.beginTransaction();
			
			//查询45号学生
		
			Student stu1=(Student) s.get(Student.class, 45);		
				tx.commit();
			
		} catch (Exception e) {
			e.printStackTrace();
			if(tx!=null){
				tx.rollback();
			}
		}finally{
			
			if(s!=null && s.isOpen()){
				s.close();
			}
		}
		
		
	}
	
	//获取一个部门
	//获取某个学生
	public static Department getDepartment() {
		// TODO Auto-generated method stub
		//通过获取一个sesion,让hibernate框架运行(config->加载hibernate.cfg.xml)
		Session s=null;
		Transaction tx=null;
		Department department=null;
		try {
			//我们使用基础模板来讲解.
			s=HibernateUtil.getCurrentSession();
			tx=s.beginTransaction();
			
			//查询3号学生
			department=(Department) s.get(Department.class, 44);
			//显示初始化代理对象
			//Hibernate.initialize(student1.getDept());
			//System.out.println(student1.getName()+" 所在部门="
			//		+student1.getDept().getName());//session还在.
			tx.commit();
			
		} catch (Exception e) {
			if(tx!=null){
				tx.rollback();
			}
		}finally{
			
			if(s!=null && s.isOpen()){
				s.close();
			}
		}
		
		return department;
	}


	//获取某个学生
	public static Student getStudent() {
		// TODO Auto-generated method stub
		//通过获取一个sesion,让hibernate框架运行(config->加载hibernate.cfg.xml)
		Session s=null;
		Transaction tx=null;
		Student student1=null;
		try {
			//我们使用基础模板来讲解.
			s=HibernateUtil.getCurrentSession();
			tx=s.beginTransaction();
			
			//查询3号学生
			student1=(Student) s.get(Student.class, 45);
			//显示初始化代理对象
			//Hibernate.initialize(student1.getDept());
			//System.out.println(student1.getName()+" 所在部门="
			//		+student1.getDept().getName());//session还在.
			tx.commit();
			
		} catch (Exception e) {
			if(tx!=null){
				tx.rollback();
			}
		}finally{
			
			if(s!=null && s.isOpen()){
				s.close();
			}
		}
		
		return student1;
	}

	public static void add() {
		// TODO Auto-generated method stub
		//通过获取一个sesion,让hibernate框架运行(config->加载hibernate.cfg.xml)
		Session s=null;
		Transaction tx=null;
		try {
			//我们使用基础模板来讲解.
			s=HibernateUtil.getCurrentSession();
			tx=s.beginTransaction();
			//添加3个学生，和部门,而且要把学生分配到该部门
			Student stu=new Student();
			stu.setName("宋江8");
			Student stu2=new Student();
			stu2.setName("林冲8");
			Department d=new Department();
			d.setName("财务部8");
			Department d2=new Department();
			d2.setName("人事部8");
			stu.setDept(d);//指定该学生是哪个部门
			stu2.setDept(d2);
			//如果先保存学生
			s.save(stu);
			s.save(d);
			s.save(stu2);
			s.save(d2);
			
			//小结论:应当先保存主对象，再保存从对象
			
			tx.commit();
			
		} catch (Exception e) {
			if(tx!=null){
				tx.rollback();
			}
		}finally{
			
			if(s!=null && s.isOpen()){
				s.close();
			}
		}
	}

}
