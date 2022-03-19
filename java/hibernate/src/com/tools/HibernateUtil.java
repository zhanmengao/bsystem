package com.tools;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.cfg.Configuration;
final public class HibernateUtil { //SqlHelper 
	private static SessionFactory sessionFactory=null;
	//ʹ���ֲ߳̾�ģʽ
	private static ThreadLocal<Session> threadLocal=new ThreadLocal<Session>();
	
	private HibernateUtil(){};
	static {
		sessionFactory=new Configuration().configure().buildSessionFactory();
	}
	
	//��ȡȫ�µ�ȫ�µ�sesession
	public static Session openSession(){
		return sessionFactory.openSession();
	}
	
	//��ȡ���̹߳�����session
	public static Session getCurrentSession(){	
		Session session=threadLocal.get();
		//�ж��Ƿ�õ�
		if(session==null){
			session=sessionFactory.openSession();
			//��session�������õ� threadLocal,�൱�ڸ�session�Ѿ����̰߳�
			threadLocal.set(session);
		}
		return session;		
	}
}