package com.domain;
//该javabean对象按照规范，应该被序列化，目的是可以唯一的标示该对象
public class Employee implements java.io.Serializable{
	private Integer myid;
	private String myname;
	private String email;
	private java.util.Date hiredate;
	public Integer getMyid() {
		return myid;
	}
	public void setMyid(Integer myid) {
		this.myid = myid;
	}
	public String getMyname() {
		return myname;
	}
	public void setMyname(String myname) {
		this.myname = myname;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public java.util.Date getHiredate() {
		return hiredate;
	}
	public void setHiredate(java.util.Date hiredate) {
		this.hiredate = hiredate;
	}

}
