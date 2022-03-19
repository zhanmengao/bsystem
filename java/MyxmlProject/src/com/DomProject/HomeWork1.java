package com.DomProject;

import java.util.Scanner;

import javax.xml.parsers.*;

import org.w3c.dom.*;

public class HomeWork1 {

	
	public static void main(String[] args) throws  Exception{
		
		Scanner scanner=new Scanner(System.in);
		StudentService studentService=new StudentService();
		//��ʾ��������
		
		System.out.println("�鿴����ѧ���ĳɼ�------view");
		System.out.println("����ѧ��id�����ѯѧ���ɼ�-----view of");
		System.out.println("���һ��ѧ��-----add");
		System.out.println("����id����һ��ѧ������Ϣ-----change");
		System.out.println("����idȥɾ��һ��ѧ��------remove");
		System.out.println("����ѧ����Ϣ-----save");
		System.out.println("�˳�ϵͳ--------exit");
		
		//��ȡ�û�ϣ����ʲô
		String operType=scanner.next();
		
		if(operType.equals("view")){
			System.out.println("����ѧ���ĳɼ�");
			studentService.showStudsFen();
		}

	}
}

//дһ��Service��
class StudentService{
	
	
	public Document getDocument(String fullFilePath) throws Exception{
		//1.�õ�DocumentBuilderFactory
		DocumentBuilderFactory documentBuilderFactory=DocumentBuilderFactory.newInstance();
			 
		//2.�õ�DocumentBuilder
		DocumentBuilder documentBuilder=documentBuilderFactory.newDocumentBuilder();
		//3.ָ���ļ�
		return documentBuilder.parse(fullFilePath);
		
	}
	
	//�����û������id����ʾѧ������Ϣ
	
	
	//��ʾ����ѧ���ɼ��ķ���[ʹ��saxдд]
	public void showStudsFen() throws Exception{
		
		//4.��ʾѧ����Ϣ
		NodeList nodeList=getDocument("src/com/dom/exercise/stus.xml").getElementsByTagName("student");
		for(int i=0;i<nodeList.getLength();i++){
			Element element=(Element) nodeList.item(i);
			//���ѧ���ı��
			System.out.println("���:\t"+element.getAttribute("sid")+
					"����\t"+getFirstElementTextVal(element, "name")+
					"java\t"+getFirstElementTextVal(element, "java")+
					"oracle\t"+getFirstElementTextVal(element, "oracle")+
					"vb\t"+getFirstElementTextVal(element, "vb"));
			
			
		}
	}
	
	public String getFirstElementTextVal(Element element,String name){
		return ((Element) element.getElementsByTagName(name).item(0)).getTextContent()+"\t\t";
	}

}


class Student{
	private String sid;
	private String name;
	private int my_java;
	private int my_oracle;
	private int my_vb;
	public String getSid() {
		return sid;
	}
	public void setSid(String sid) {
		this.sid = sid;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getMy_java() {
		return my_java;
	}
	public void setMy_java(int my_java) {
		this.my_java = my_java;
	}
	public int getMy_oracle() {
		return my_oracle;
	}
	public void setMy_oracle(int my_oracle) {
		this.my_oracle = my_oracle;
	}
	public int getMy_vb() {
		return my_vb;
	}
	public void setMy_vb(int my_vb) {
		this.my_vb = my_vb;
	}
}

