package com.sina.domain;

import java.util.HashSet;
import java.util.Set;

/**
 * Type entity.
 * 
 * @author MyEclipse Persistence Tools
 */

public class Type implements java.io.Serializable {

	// Fields

	private Integer id;
	private String type;
	private Set contents = new HashSet(0);

	// Constructors

	/** default constructor */
	public Type() {
	}

	/** full constructor */
	public Type(String type, Set contents) {
		this.type = type;
		this.contents = contents;
	}

	// Property accessors

	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getType() {
		return this.type;
	}

	public void setType(String type) {
		this.type = type;
	}

	public Set getContents() {
		return this.contents;
	}

	public void setContents(Set contents) {
		this.contents = contents;
	}

}