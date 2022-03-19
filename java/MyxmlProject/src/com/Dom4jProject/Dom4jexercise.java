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
		//���Document
		Document document=getDocument("src/classes.xml");
		//���в˵�
		dom4jexercise.menu(document);
		
	}
	//���ܣ����Document
	public static Document getDocument(String url) throws Exception
	{
		SAXReader reader=new SAXReader();
		Document doc=reader.read(new File(url));
		return doc;
	}
	//���ܣ����˵�
	public void menu(Document document)
	{
		Element root=document.getRootElement();
		ManagerClass managerclass=new ManagerClass();
		boolean b = false;
		while(true)
		{
			System.out.println();
			System.out.println("view:�鿴����ѧ���ɼ�");
			System.out.println("view of:����ID�鿴ѧ���ɼ�");
			System.out.println("add:���һ��ѧ��");
			System.out.println("change:����ID�޸�ѧ����Ϣ");
			System.out.println("remode:����IDɾ��ѧ��");
			System.out.println("save:����ѧ����Ϣ");
			System.out.println("exit:�˳�ϵͳ");
			
			BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			try {
				String s=br.readLine();
				if(s.equals("view"))
				{
					//�û���Ҫ�鿴����ѧ��
					b=managerclass.list(root);
				}
				else if(s.equals("view of"))
				{
					//�û���Ҫ�鿴ĳ��ѧ��
					System.out.println("��������Ҫ�鿴��ѧ��ѧ��");
					String sid=br.readLine();	
					b=managerclass.select(root, sid,"java");
				}
				else if(s.equals("add"))
				{
					//�û���Ҫ���ѧ��
					System.out.println("�������µ�ѧ��ѧ��");
					String sid=br.readLine();
					System.out.println("������ѧ������");
					String name=br.readLine();
					System.out.println("������ѧ��java�γ̳ɼ�");
					String java=br.readLine();
					System.out.println("������ѧ��oracle�γ̳ɼ�");
					String oracle=br.readLine();
					System.out.println("������ѧ��vb�γ̳ɼ�");
					String vb=br.readLine();
					Student NewStu=new Student();
					NewStu.setSid(sid);
					NewStu.setName(name);
					NewStu.setJava(java);
					NewStu.setOracle(oracle);
					NewStu.setVb(vb);
					//ִ�����
					b=managerclass.add(root, NewStu);

				}
				else if(s.equals("change"))
				{
					//�û���Ҫ����
					System.out.println("������Ҫ���ĵ�ѧ��ѧ��");
					String sid=br.readLine();
					System.out.println("������ѧ������");
					String name=br.readLine();
					System.out.println("������ѧ��java�γ̳ɼ�");
					String java=br.readLine();
					System.out.println("������ѧ��oracle�γ̳ɼ�");
					String oracle=br.readLine();
					System.out.println("������ѧ��vb�γ̳ɼ�");
					String vb=br.readLine();
					Student NewStu=new Student();
					NewStu.setSid(sid);
					NewStu.setName(name);
					NewStu.setJava(java);
					NewStu.setOracle(oracle);
					NewStu.setVb(vb);
					//ִ���޸�
					b=managerclass.update(root, NewStu);

				}
				else if(s.equals("remove"))
				{
					//�û���Ҫɾ��
					System.out.println("����������Ҫɾ����ѧ��ѧ��");
					String sid=br.readLine();
					b=managerclass.delete(root,"student" , sid);

				}
				else if(s.equals("save"))
				{
					//�û���Ҫ����
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
				System.out.println("�������!");
			}else{
				System.out.println("����ʧ�ܣ�");
			}
			b=false;
		}
	}
}
//����ϵͳ������
class ManagerClass{
	//���ܣ���ѧ�Ÿ���ѧ����Ϣ
	//���룺ѧ��
	public static boolean update(Element element,Student stu)
	{
		//�����ǰѧ����Ϣ//student[@id='stu.getSid']
		Element Stu_MyCare=(Element) element.selectNodes("//student[@sid='"+stu.getSid()+"']").get(0);
		list(Stu_MyCare);
		System.out.println("�Ƿ��޸ĸ�ѧ����Y/N");
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
	
	//����:��ѧ��ɾ��
	public static boolean delete(Element element,String name,String text) throws Exception
	{
		//�õ����з��������ı�ǩ //student[@sid='text']
		List<Element> List_MyCare=element.selectNodes("//"+name+"[@sid='"+text+"']");
		//��ӡȡ��������ѧ��
		for(int i=0;i<List_MyCare.size();i++)
		{
			Element stu=List_MyCare.get(i);
			//��ӡѧ����Ϣ
			list(stu);
			System.out.println();
			System.out.println("�Ƿ�ɾ����ѧ��?Y/N");
			BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			String br_s=br.readLine();
			//ִ��ɾ������
			if(br_s.equals("Y"))
			{
				element.remove(stu);
			}
		}
		return true;
	}
	//���ܣ����ѧ����xml
	//���룺���ڵ㣬�Լ�ѧ������
	public static boolean add(Element element,Student stu)
	{
		//����һ��ѧ��
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
		//������
		NewStu.add(NewStu_name);
		NewStu.add(NewStu_course);
		NewStu_course.add(NewStu_course_java);
		NewStu_course.add(NewStu_course_oracle);
		NewStu_course.add(NewStu_course_vb);
		element.add(NewStu);
		return true;
	}
	//���ܣ���ѯĳ��ѧ����ĳ��Ԫ��
	//���룺���ڵ㣬ѧ�ź͹��ĵ��ӱ�ǩ����
	public static boolean select(Element element,String sid,String name)
	{
		//�ҵ���ѧ��
		List StuList=element.selectNodes("//student[@sid='"+sid+"']");
		//�ҵ���ѧ���µ�ָ������
		List Stu_MyCare=((Element)StuList.get(0)).selectNodes("//"+name);
		//��ӡ��ѧ��ѧ���Լ�����
		System.out.println("ѧ��"+sid+"  "+name+"Ϊ��"+((Element)Stu_MyCare.get(0)).getTextTrim());
		
		return true;
	}
	//���ܣ����������ڵ��µ������ӽڵ�
	//���룺��ǰ���ڵ�
	//���ܣ�������ǰ�ڵ��µ�������Ϣ
	//���룺����Ҫ������ͷ���
	public static boolean list(Element element)
	{
		if(element.getName().equals("student")||element.getName().equals("java")
				||element.getName().equals("oracle")||element.getName().equals("vb")
				||element.getName().equals("name"))
		{
			if(element.getName().equals("student"))
			{
				System.out.println();
				//��ӡ����
				System.out.print("ѧ��"+element.attributeValue("sid")+"\t\t");
			}
			else
			{
				//��ӡ��ǰԪ����Ϣ
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
	
	//���ܣ����ڴ���µ��ļ�
	//���룺��Ҫ�����Document�ĵ�
	public static boolean save(Document document) throws Exception
	{
		//ֱ�����������������루dom4j��������sun��˾��������ȫ�棩
		OutputFormat outputformat=OutputFormat.createPrettyPrint();
		outputformat.setEncoding("utf-8");//�����������
		
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





















