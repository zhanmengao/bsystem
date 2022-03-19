package com.sina.domain;

public class Message {

	private Integer mesId;
	private java.util.Date mesTime;
	private String content;
	//¶ÔÏó
	private Users sender;
	private Users getter;
	public Integer getMesId() {
		return mesId;
	}
	public void setMesId(Integer mesId) {
		this.mesId = mesId;
	}
	public java.util.Date getMesTime() {
		return mesTime;
	}
	public void setMesTime(java.util.Date mesTime) {
		this.mesTime = mesTime;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}
	public Users getSender() {
		return sender;
	}
	public void setSender(Users sender) {
		this.sender = sender;
	}
	public Users getGetter() {
		return getter;
	}
	public void setGetter(Users getter) {
		this.getter = getter;
	}
}
