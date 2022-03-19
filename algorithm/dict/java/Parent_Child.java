package com.tree;
/*
 * 定义通用树：孩子节点用next指针连接
 */

public class Parent_Child {
	private Object data;//自己的数据
	private Parent_Child parent;//双亲节点的位置
	private Parent_Child child_head;//第一个孩子
	private Parent_Child next;//下一个兄弟
	public Object getData() {
		return data;
	}
	public void setData(Object data) {
		this.data = data;
	}
	public Parent_Child getParent() {
		return parent;
	}
	public void setParent(Parent_Child parent) {
		this.parent = parent;
	}
	public Parent_Child getChild_head() {
		return child_head;
	}
	public void setChild_head(Parent_Child child_head) {
		this.child_head = child_head;
	}
	public Parent_Child getNext() {
		return next;
	}
	public void setNext(Parent_Child next) {
		this.next = next;
	}

}

