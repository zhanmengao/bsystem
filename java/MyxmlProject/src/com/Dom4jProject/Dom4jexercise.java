package com.Dom4jProject;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.Iterator;
import java.util.List;
import java.util.Scanner;

import org.dom4j.*;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;


public class Dom4jexercise {
	public static void main(String[] args) throws Exception{
		// TODO Auto-generated method stub
		Dom4jexercise dom4jexercise=new Dom4jexercise();
		//获得Document
		Document document=getDocument("src/classes.xml");
		//运行菜单
		dom4jexercise.menu(document);
		
	}
	//功能：获得Document
	public static Document getDocument(String url) throws Exception
	{
		SAXReader reader=new SAXReader();
		Document doc=reader.read(new File(url));
		return doc;
	}
	//功能：主菜单
	public void menu(Document document)
	{
		Element root=document.getRootElement();
		ManagerClass managerclass=new ManagerClass();
		boolean b = false;
		while(true)
		{
			System.out.println();
			System.out.println("view:查看所有学生成绩");
			System.out.println("view of:按照ID查看学生成绩");
			System.out.println("add:添加一个学生");
			System.out.println("change:按照ID修改学生信息");
			System.out.println("remode:按照ID删除学生");
			System.out.println("save:保存学生信息");
			System.out.println("exit:退出系统");
			
			BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			try {
				String s=br.readLine();
				if(s.equals("view"))
				{
					//用户想要查看所有学生
					b=managerclass.list(root);
				}
				else if(s.equals("view of"))
				{
					//用户想要查看某个学生
					System.out.println("请输入您要查看的学生学号");
					String sid=br.readLine();	
					b=managerclass.select(root, sid,"java");
				}
				else if(s.equals("add"))
				{
					//用户想要添加学生
					System.out.println("请输入新的学生学号");
					String sid=br.readLine();
					System.out.println("请输入学生姓名");
					String name=br.readLine();
					System.out.println("请输入学生java课程成绩");
					String java=br.readLine();
					System.out.println("请输入学生oracle课程成绩");
					String oracle=br.readLine();
					System.out.println("请输入学生vb课程成绩");
					String vb=br.readLine();
					Student NewStu=new Student();
					NewStu.setSid(sid);
					NewStu.setName(name);
					NewStu.setJava(java);
					NewStu.setOracle(oracle);
					NewStu.setVb(vb);
					//执行添加
					b=managerclass.add(root, NewStu);

				}
				else if(s.equals("change"))
				{
					//用户想要更改
					System.out.println("请输入要更改的学生学号");
					String sid=br.readLine();
					System.out.println("请输入学生姓名");
					String name=br.readLine();
					System.out.println("请输入学生java课程成绩");
					String java=br.readLine();
					System.out.println("请输入学生oracle课程成绩");
					String oracle=br.readLine();
					System.out.println("请输入学生vb课程成绩");
					String vb=br.readLine();
					Student NewStu=new Student();
					NewStu.setSid(sid);
					NewStu.setName(name);
					NewStu.setJava(java);
					NewStu.setOracle(oracle);
					NewStu.setVb(vb);
					//执行修改
					b=managerclass.update(root, NewStu);

				}
				else if(s.equals("remove"))
				{
					//用户想要删除
					System.out.println("请输入您想要删除的学生学号");
					String sid=br.readLine();
					b=managerclass.delete(root,"student" , sid);

				}
				else if(s.equals("save"))
				{
					//用户想要保存
					b=managerclass.save(document);

				}
				else if(s.equals("exit"))
				{
					System.exit(0);
				}
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			System.out.println();
			if(b)
			{
				System.out.println("操作完成!");
			}else{
				System.out.println("操作失败！");
			}
			b=false;
		}
	}
}
//教务系统管理类
class ManagerClass{
	//功能：按学号更新学生信息
	//输入：学号
	public static boolean update(Element element,Student stu)
	{
		//输出当前学生信息//student[@id='stu.getSid']
		Element Stu_MyCare=(Element) element.selectNodes("//student[@sid='"+stu.getSid()+"']").get(0);
		list(Stu_MyCare);
		System.out.println("是否修改该学生？Y/N");
		Scanner scanner=new Scanner(System.in);
		if(scanner.next().equals("Y"))
		{
			Element Stu_Name=(Element) Stu_MyCare.selectNodes("//name").get(0);
			Stu_Name.setText(stu.getName());
			List Stu_Course=Stu_MyCare.selectNodes("course/*");
			((Element)Stu_Course.get(0)).setText(stu.getJava());
			((Element)Stu_Course.get(1)).setText(stu.getOracle());
			((Element)Stu_Course.get(2)).setText(stu.getVb());
			
		}
		
		return true;
	}
	
	//功能:按学号删除
	public static boolean delete(Element element,String name,String text) throws Exception
	{
		//拿到所有符合条件的标签 //student[@sid='text']
		List<Element> List_MyCare=element.selectNodes("//"+name+"[@sid='"+text+"']");
		//打印取到的所有学生
		for(int i=0;i<List_MyCare.size();i++)
		{
			Element stu=List_MyCare.get(i);
			//打印学生信息
			list(stu);
			System.out.println();
			System.out.println("是否删除该学生?Y/N");
			BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			String br_s=br.readLine();
			//执行删除操作
			if(br_s.equals("Y"))
			{
				element.remove(stu);
			}
		}
		return true;
	}
	//功能：添加学生到xml
	//输入：根节点，以及学生对象
	public static boolean add(Element element,Student stu)
	{
		//创建一个学生
		Element NewStu=DocumentHelper.createElement("student");
		NewStu.addAttribute("sid",stu.getSid());
		Element NewStu_name=DocumentHelper.createElement("name");
		NewStu_name.setText(stu.getName());
		Element NewStu_course=DocumentHelper.createElement("course");
		Element NewStu_course_java=DocumentHelper.createElement("java");
		NewStu_course_java.setText(stu.getJava());
		Element NewStu_course_oracle=DocumentHelper.createElement("oracle");
		NewStu_course_oracle.setText(stu.getOracle());
		Element NewStu_course_vb=DocumentHelper.createElement("vb");
		NewStu_course_vb.setText(stu.getVb());
		//连成树
		NewStu.add(NewStu_name);
		NewStu.add(NewStu_course);
		NewStu_course.add(NewStu_course_java);
		NewStu_course.add(NewStu_course_oracle);
		NewStu_course.add(NewStu_course_vb);
		element.add(NewStu);
		return true;
	}
	//功能：查询某个学生的某个元素
	//输入：根节点，学号和关心的子标签名字
	public static boolean select(Element element,String sid,String name)
	{
		//找到该学生
		List StuList=element.selectNodes("//student[@sid='"+sid+"']");
		//找到该学生下的指定属性
		List Stu_MyCare=((Element)StuList.get(0)).selectNodes("//"+name);
		//打印该学生学号以及属性
		System.out.println("学号"+sid+"  "+name+"为："+((Element)Stu_MyCare.get(0)).getTextTrim());
		
		return true;
	}
	//功能：遍历所给节点下的所有子节点
	//输入：当前最大节点
	//功能：遍历当前节点下的所有信息
	//输入：您所要遍历的头结点
	public static boolean list(Element element)
	{
		if(element.getName().equals("student")||element.getName().equals("java")
				||element.getName().equals("oracle")||element.getName().equals("vb")
				||element.getName().equals("name"))
		{
			if(element.getName().equals("student"))
			{
				System.out.println();
				//打印属性
				System.out.print("学号"+element.attributeValue("sid")+"\t\t");
			}
			else
			{
				//打印当前元素信息
				System.out.print(element.getName()+element.getTextTrim()+"\t\t");
			}

		}
		Iterator iter=element.elementIterator();
		while(iter.hasNext())
		{
			Element ChildElement=(Element) iter.next();
			list(ChildElement);
		}
		return true;
	}
	
	//功能：从内存更新到文件
	//输入：您要保存的Document文档
	public static boolean save(Document document) throws Exception
	{
		//直接输出会出现中文乱码（dom4j并不会像sun公司那样考虑全面）
		OutputFormat outputformat=OutputFormat.createPrettyPrint();
		outputformat.setEncoding("utf-8");//设置输出编码
		
        // lets write to a file
        XMLWriter writer = new XMLWriter(
            new FileOutputStream( "src/classes.xml" ),outputformat
        );
        writer.write( document );
        writer.close();
        return true;
	}
}
class Student{
	private String sid;
	private String name;
	private String java;
	private String oracle;
	private String vb;
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
	public String getJava() {
		return java;
	}
	public void setJava(String java) {
		this.java = java;
	}
	public String getOracle() {
		return oracle;
	}
	public void setOracle(String oracle) {
		this.oracle = oracle;
	}
	public String getVb() {
		return vb;
	}
	public void setVb(String vb) {
		this.vb = vb;
	}
}





















