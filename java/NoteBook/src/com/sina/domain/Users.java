package com.sina.domain;

import java.util.List;
import java.util.Set;

public class Users {

	private Integer userid;
	private String userpwd;
	private String name;
	//这里一个用户可以发送多个消息，也可以接收多个消息. one-to-many
	
	private Set<Message> sendMessages;
	private Set<Message> getMessages;
	public Integer getUserid() {
		return userid;
	}
	public void setUserid(Integer userid) {
		this.userid = userid;
	}
	public String getUserpwd() {
		return userpwd;
	}
	public void setUserpwd(String userpwd) {
		this.userpwd = userpwd;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public Set<Message> getSendMessages() {
		return sendMessages;
	}
	public void setSendMessages(Set<Message> sendMessages) {
		this.sendMessages = sendMessages;
	}
	public Set<Message> getGetMessages() {
		return getMessages;
	}
	public void setGetMessages(Set<Message> getMessages) {
		this.getMessages = getMessages;
	}
	
	
}
