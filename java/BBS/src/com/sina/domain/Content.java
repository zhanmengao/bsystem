package com.sina.domain;

/**
 * Content entity.
 * 
 * @author MyEclipse Persistence Tools
 */

public class Content implements java.io.Serializable {

	// Fields

	private Integer id;
	private Type type;
	private String name;
	private String sex;
	private String content;

	// Constructors

	/** default constructor */
	public Content() {
	}

	/** minimal constructor */
	public Content(String name, String sex) {
		this.name = name;
		this.sex = sex;
	}

	/** full constructor */
	public Content(Type type, String name, String sex, String content) {
		this.type = type;
		this.name = name;
		this.sex = sex;
		this.content = content;
	}

	// Property accessors

	public Integer getId() {
		return this.id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public Type getType() {
		return this.type;
	}

	public void setType(Type type) {
		this.type = type;
	}

	public String getName() {
		return this.name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getSex() {
		return this.sex;
	}

	public void setSex(String sex) {
		this.sex = sex;
	}

	public String getContent() {
		return this.content;
	}

	public void setContent(String content) {
		this.content = content;
	}

}