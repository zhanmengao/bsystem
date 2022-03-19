package com.Dom4jProject;
/*
 * dom4j��CRUD����
 */
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import org.dom4j.*;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;

public class Mydom4jTest {

	public static void main(String[] args) throws Exception {
		// TODO Auto-generated method stub
		//�õ�������
		SAXReader reader=new SAXReader();
		//�ý�����ȥ���ĵ�
		Document dom4j_document=reader.read(new File("src/com/dom4j/classes.xml"));
		//�õ���Ԫ��(students)
		Element root=dom4j_document.getRootElement();
		addToIndex(root);
		save(dom4j_document);
		
	}
	//���ܣ������ĵ�����
	public static void save(Document document) throws Exception
	{
		//ֱ�����������������루dom4j��������sun��˾��������ȫ�棩
		OutputFormat outputformat=OutputFormat.createPrettyPrint();
		outputformat.setEncoding("utf-8");//�����������
		
        // lets write to a file
        XMLWriter writer = new XMLWriter(
            new FileOutputStream( "src/com/dom4j/classes.xml" ),outputformat);
        writer.write( document );
        writer.close();
	}
	//���ܣ�ɾ��Ԫ��
	public static void delete(Element element)
	{
		//ɾ��������ѧ��
		element.remove((Element)element.elements("ѧ��").get(2));
		//ɾ���ڶ���ѧ���ı���(����)
		element.remove(((Element)element.elements("ѧ��").get(1)).attribute("����"));
		
	}
	//���ܣ��޸�Ԫ��(Ҫ�������ѧ��������+3)
	public static void update(Element element)
	{
		//�õ�����ѧ���ڵ�
		List<Element> StuList=element.elements("ѧ��");
		//�õ�����ѧ���ڵ��µ� ���� �ڵ㣬������+3
		//���õ��������ԣ��������ı����ǵ�ֵ
		for(int i=0;i<StuList.size();i++)
		{
			StuList.get(i).addAttribute("����", "����");
			Element Stu_age=StuList.get(i).element("����");
			Stu_age.setText(Integer.parseInt(Stu_age.getTextTrim())+3+"");
		}
	}
	//���ܣ����һ��ѧ�������ǳۺ���
	public static void addToIndex(Element element)
	{
		//����һ��ѧ��
		Element NewStu=DocumentHelper.createElement("ѧ��");
		NewStu.setText("������");
		//���
		List<Element> StuList=element.elements("ѧ��");
		StuList.add(1,NewStu);
	}
	//���ܣ����һ��ѧ��
	public static void add(Element element)
	{
		//�½�һ����ǩ��DocumentHelper
		Element newStu=DocumentHelper.createElement("ѧ��");
		newStu. addAttribute("����", "ɵ��");
		Element newStu_name=DocumentHelper.createElement("����");
		Element newStu_age=DocumentHelper.createElement("����");
		Element newStu_intro=DocumentHelper.createElement("����");
		
		//������:add
		newStu.add(newStu_name);
		newStu.add(newStu_age);
		newStu.add(newStu_intro);
		//��ӵ���������Ԫ����
		element.add(newStu);
	}
	//���ܣ�����ĳһ����ǩ
	public static void select(Element element,String Page_name)
	{
		//root.elements("ѧ��") :��ʾȡ�� root Ԫ�� �µ�����ѧ��Ԫ��
		//root.element("ѧ��") :��ʾȡ�� root Ԫ�� �µĵ�һ��ѧ��Ԫ��
		//root.elements("ѧ��").get(0) : ��ʾȡ�� root Ԫ�� �µĵ�һ��ѧ��Ԫ��
		Element stu1=(Element) element.elements(Page_name).get(0);
		System.out.println(stu1.getName()+stu1.getTextTrim());
		Element name=stu1.element("����");
		System.out.println(name.getTextTrim());
		
		//ȡ����ǩ������
		String littername=name.attributeValue("����");
		System.out.println(littername);
	}
	
	//���ܣ�����xml�ĵ�
	public static void list(Element element)
	{
		//�����ǰԪ����Ϣ
		System.out.println(element.getName()+element.getTextTrim());
		//�õ��ñ�ǩ�ĵ�����
		Iterator iter=element.elementIterator();
		while(iter.hasNext())
		{
			//ȡ����һ��Ԫ��
			Element element_2=(Element) iter.next();
			//�ݹ�
			list(element_2);
		}
	}

}







