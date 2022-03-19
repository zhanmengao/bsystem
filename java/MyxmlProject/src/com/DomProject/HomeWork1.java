package com.DomProject;

import java.util.Scanner;

import javax.xml.parsers.*;

import org.w3c.dom.*;

public class HomeWork1 {

	
	public static void main(String[] args) throws  Exception{
		
		Scanner scanner=new Scanner(System.in);
		StudentService studentService=new StudentService();
		//显示操作界面
		
		System.out.println("查看所有学生的成绩------view");
		System.out.println("按照学生id号码查询学生成绩-----view of");
		System.out.println("添加一个学生-----add");
		System.out.println("按照id更改一个学生的信息-----change");
		System.out.println("按照id去删除一个学生------remove");
		System.out.println("保存学生信息-----save");
		System.out.println("退出系统--------exit");
		
		//获取用户希望干什么
		String operType=scanner.next();
		
		if(operType.equals("view")){
			System.out.println("所有学生的成绩");
			studentService.showStudsFen();
		}

	}
}

//写一个Service类
class StudentService{
	
	
	public Document getDocument(String fullFilePath) throws Exception{
		//1.得到DocumentBuilderFactory
		DocumentBuilderFactory documentBuilderFactory=DocumentBuilderFactory.newInstance();
			 
		//2.得到DocumentBuilder
		DocumentBuilder documentBuilder=documentBuilderFactory.newDocumentBuilder();
		//3.指定文件
		return documentBuilder.parse(fullFilePath);
		
	}
	
	//根据用户输入的id来显示学生的信息
	
	
	//显示所有学生成绩的方法[使用sax写写]
	public void showStudsFen() throws Exception{
		
		//4.显示学生信息
		NodeList nodeList=getDocument("src/com/dom/exercise/stus.xml").getElementsByTagName("student");
		for(int i=0;i<nodeList.getLength();i++){
			Element element=(Element) nodeList.item(i);
			//输出学生的编号
			System.out.println("编号:\t"+element.getAttribute("sid")+
					"名字\t"+getFirstElementTextVal(element, "name")+
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

