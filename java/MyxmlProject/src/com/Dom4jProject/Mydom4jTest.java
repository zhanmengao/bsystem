package com.Dom4jProject;
/*
 * dom4j的CRUD操作
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
		//得到解析器
		SAXReader reader=new SAXReader();
		//用解析器去读文档
		Document dom4j_document=reader.read(new File("src/com/dom4j/classes.xml"));
		//拿到根元素(students)
		Element root=dom4j_document.getRootElement();
		addToIndex(root);
		save(dom4j_document);
		
	}
	//功能：更新文档内容
	public static void save(Document document) throws Exception
	{
		//直接输出会出现中文乱码（dom4j并不会像sun公司那样考虑全面）
		OutputFormat outputformat=OutputFormat.createPrettyPrint();
		outputformat.setEncoding("utf-8");//设置输出编码
		
        // lets write to a file
        XMLWriter writer = new XMLWriter(
            new FileOutputStream( "src/com/dom4j/classes.xml" ),outputformat);
        writer.write( document );
        writer.close();
	}
	//功能：删除元素
	public static void delete(Element element)
	{
		//删除第三个学生
		element.remove((Element)element.elements("学生").get(2));
		//删除第二个学生的别名(属性)
		element.remove(((Element)element.elements("学生").get(1)).attribute("别名"));
		
	}
	//功能：修改元素(要求把所有学生的年龄+3)
	public static void update(Element element)
	{
		//拿到所有学生节点
		List<Element> StuList=element.elements("学生");
		//拿到所有学生节点下的 年龄 节点，让它们+3
		//再拿到所有属性：别名，改变它们的值
		for(int i=0;i<StuList.size();i++)
		{
			StuList.get(i).addAttribute("别名", "至尊宝");
			Element Stu_age=StuList.get(i).element("年龄");
			Stu_age.setText(Integer.parseInt(Stu_age.getTextTrim())+3+"");
		}
	}
	//功能：添加一个学生到周星驰后面
	public static void addToIndex(Element element)
	{
		//创建一个学生
		Element NewStu=DocumentHelper.createElement("学生");
		NewStu.setText("王必宇");
		//添加
		List<Element> StuList=element.elements("学生");
		StuList.add(1,NewStu);
	}
	//功能：添加一个学生
	public static void add(Element element)
	{
		//新建一个标签：DocumentHelper
		Element newStu=DocumentHelper.createElement("学生");
		newStu. addAttribute("别名", "傻子");
		Element newStu_name=DocumentHelper.createElement("名字");
		Element newStu_age=DocumentHelper.createElement("年龄");
		Element newStu_intro=DocumentHelper.createElement("介绍");
		
		//连起来:add
		newStu.add(newStu_name);
		newStu.add(newStu_age);
		newStu.add(newStu_intro);
		//添加到传进来的元素下
		element.add(newStu);
	}
	//功能：查找某一个标签
	public static void select(Element element,String Page_name)
	{
		//root.elements("学生") :表示取出 root 元素 下的所有学生元素
		//root.element("学生") :表示取出 root 元素 下的第一个学生元素
		//root.elements("学生").get(0) : 表示取出 root 元素 下的第一个学生元素
		Element stu1=(Element) element.elements(Page_name).get(0);
		System.out.println(stu1.getName()+stu1.getTextTrim());
		Element name=stu1.element("名字");
		System.out.println(name.getTextTrim());
		
		//取出标签的属性
		String littername=name.attributeValue("别名");
		System.out.println(littername);
	}
	
	//功能：遍历xml文档
	public static void list(Element element)
	{
		//打出当前元素信息
		System.out.println(element.getName()+element.getTextTrim());
		//拿到该标签的迭代器
		Iterator iter=element.elementIterator();
		while(iter.hasNext())
		{
			//取出下一个元素
			Element element_2=(Element) iter.next();
			//递归
			list(element_2);
		}
	}

}







