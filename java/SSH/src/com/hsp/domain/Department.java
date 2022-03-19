package com.hsp.domain;

import java.util.Set;

public class Department {

	private Integer id;
	private String name;
	
	public Department(){
		
	}
	
	private Set<Employee> emps;
	public Set<Employee> getEmps() {
		return emps;
	}
	public void setEmps(Set<Employee> emps) {
		this.emps = emps;
	}
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
	public Department(String name) {
		
		this.name = name;
	}
}
