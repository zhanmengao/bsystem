package com.tree;
/*
 * ����ͨ���������ӽڵ���nextָ������
 */

public class Parent_Child {
	private Object data;//�Լ�������
	private Parent_Child parent;//˫�׽ڵ��λ��
	private Parent_Child child_head;//��һ������
	private Parent_Child next;//��һ���ֵ�
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

