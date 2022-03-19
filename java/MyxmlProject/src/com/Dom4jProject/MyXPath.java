package com.Dom4jProject;

import java.io.File;
import java.util.List;

import org.dom4j.*;
import org.dom4j.io.*;

public class MyXPath {
	public static void main(String []args) throws Exception
	{
		SAXReader reader=new SAXReader();
		Document doc=reader.read(new File("src/com/dom4jProject/XPath.xml"));
		select(doc.getRootElement());
		
	}
	//取数据
	public static void select(Element root)
	{
		//基本的XPath语法类似于在一个文件系统中定位文件,
		//如果路径以斜线 / 开始, 那么该路径就表示到一个元素的绝对路径
		List list=root.selectNodes("/AAA");
	
		//如果路径以双斜线 // 开头, 则表示选择文档中所有满足双斜线//之后规则的元素(无论层级关系)
		list=root.selectNodes("//AAA");
		
		//星号 * 表示选择所有由星号之前的路径所定位的元素
		list=root.selectNodes("/AAA/CCC/DDD/*");
		
		//方块号里的表达式可以进一步的指定元素, 其中数字表示元素在选择集里的位置, 而last()函数则表示选择集中的最后一个元素.
		list=root.selectNodes("/AAA/BBB[1]");
		
		//选择含有属性id且其值为'b1'的BBB元素
		list=root.selectNodes("//BBB[@id='b1']");
		
		//count()函数可以计数所选元素的个数
		list=root.selectNodes("//*[count(BBB)=2]");//选择含有2个BBB子元素的元素
		
		//name()函数返回元素的名称, 
		//start-with()函数在该函数的第一个参数字符串是以第二个参数字符开始的情况返回true, 
		//contains()函数当其第一个字符串参数包含有第二个字符串参数时返回true.
		list=root.selectNodes("//*[name()='BBB']");
		list=root.selectNodes("//*[starts-with(name(),'B')]");//选择所有名称以"B"起始的元素
		list=root.selectNodes("//*[contains(name(),'C')]");//选择所有名称包含"C"的元素
		
		System.out.println(list.size());
		
	}
}
