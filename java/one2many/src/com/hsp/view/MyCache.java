package com.hsp.view;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MyCache {

	
	//使用map来模拟缓存
	static Map<Integer,Student> maps=new HashMap<Integer,Student>();
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		getStudent(1);
		getStudent(1);
		getStudent(1);
		getStudent(1);
		getStudent(3);
		getStudent(3);
		
		
		
	}
	
	public static Student getStudent(Integer id){  //s.get()
		
		
		//先到缓存去
		if(maps.containsKey(id)){
			//在缓存有
			System.out.println("从缓存取出");
			return maps.get(id);
		}else{
			System.out.println("从数据库中取");
			//到数据库取
			Student stu=MyDB.getStudentFromDB(id);
			//放入缓存
			maps.put(id, stu);
			return stu;
		}
		
	
		
	}

}

//我的数据库
class MyDB{
	
	static List<Student> lists=new  ArrayList<Student>();
	
	//初始化数据库,假设有三个学生
	static{
		Student s1=new Student();
		s1.setId(1);
		s1.setName("aaa");
		Student s2=new Student();
		s2.setId(2);
		s2.setName("bbb");
		Student s3=new Student();
		s3.setId(3);
		s3.setName("ccc");
		lists.add(s1);
		lists.add(s2);
		lists.add(s3);
		
	}
	
	public static Student getStudentFromDB(Integer id){
		for(Student s: lists){
			if(s.getId().equals(id)){
				return s;
			}
		}
		return null;// 在数据库中没有.
		
	}
}

class Student{
	private Integer id;
	private String name;
	public Integer getId() {
		return id;
	}
	public void setId(Integer id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
}
