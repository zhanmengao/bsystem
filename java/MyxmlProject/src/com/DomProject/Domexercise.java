package com.DomProject;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import org.w3c.dom.Document;

/*
 * 完成对学生表xml的CRUD
 */

public class Domexercise {

	public static void main(String[] args) throws Exception{
		// TODO Auto-generated method stub
		Domexercise cc=new Domexercise();
		//获得Document
		Document document=getDocument("src/com/DomProject/classes.xml");
		//运行菜单
		cc.menu(document);
		
	}
	//功能：获得Document
	public static Document getDocument(String url)
	{
		Document doc=null;
		try {
			//获取dom解析器的工厂
			DocumentBuilderFactory DBF=DocumentBuilderFactory.newInstance();
			//得到新的dom解析器对象
			DocumentBuilder DBuilder=DBF.newDocumentBuilder();
			//让DocumentBuilder去解析文档，拿到doc对象
			doc=DBuilder.parse(url);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return doc;
	}
	//功能：主菜单
	public void menu(Document d)
	{
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
					//拿到students
					Element e=d.getDocumentElement();
					//用户想要查看所有学生
					SelectAll(e);
				}
				else if(s.equals("view of"))
				{
					//用户想要查看某个学生
					System.out.println("请输入您要查看的学生学号");
					String sid=br.readLine();	
					Select(d, sid);
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
					//执行添加
					b=add(d, sid, name, java, oracle, vb);
				}
				else if(s.equals("change"))
				{
					//用户想要更改
					System.out.println("请输入要更改的学生学号");
					String sid=br.readLine();
					try {
						b=update(d, sid);
					} catch (Exception e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				else if(s.equals("remove"))
				{
					//用户想要删除
					System.out.println("请输入您想要删除的学生学号");
					String sid=br.readLine();
					b=delete(d, sid);
				}
				else if(s.equals("save"))
				{
					//用户想要保存
					b=shuaxin(d);
				}
				else if(s.equals("exit"))
				{
					System.exit(0);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			if(b)
			{
				System.out.println("操作完成!");
			}else{
				System.out.println("操作失败！");
			}
		}
	}
	
	//功能：完成对dom的遍历，并打印所有学生信息
	//输入：头结点Node
	public static void SelectAll(Element n)
	{
		//如果不是容器节点
		if(n.getNodeType()==n.ELEMENT_NODE && !n.getNodeName().equals("course") && !n.getNodeName().equals("students") )
		{
			//输出节点的属性、元素名和文字内容
			//如果该节点有学号属性（student节点）
			if(!n.getAttribute("sid").equals(""))
			{
				System.out.println();
				System.out.println("学生学号"+n.getAttribute("sid")+"\t\t");
			}
			else
			{
				//输出各标签Text
				System.out.print(n.getNodeName()+"为"+n.getTextContent()+"\t\t");
			}	
		}
		//再取得n的子节点
		NodeList nl=n.getChildNodes();
		//让子节点在执行相同的操作
		for(int i=0;i<nl.getLength();i++)
		{
			//取出子节点
			if(nl.item(i).getNodeType()==Node.ELEMENT_NODE)
			{
				Element nc=(Element) nl.item(i);
				//让子节点再次取出它的子节点
				SelectAll(nc);
			}
		}
	}
	
	public static void Select(Document doc,String StuId)
	{
		//遍历学生节点 找到所有学生
		NodeList nl=doc.getElementsByTagName("student");
		//取出所有学生，并匹配属性
		for(int i=0;i<nl.getLength();i++)
		{
			//取出该学生
			Element e=(Element)nl.item(i);
			//匹配
			if(e.getAttribute("sid").equals(StuId))
			{
				//匹配对了 输出信息
				SelectAll(e);
			}
		}	
	}
	
	//功能：添加一个学生
	//输入：doc,stuid,name,java,oracle,vb
	//输出：成功与否
	public static boolean add(org.w3c.dom.Document doc,String stuid,String name,String java,String oracle,String vb)
	{
		boolean b=false;
		//创建一个新的学生节点
		Element e=doc.createElement("student");
		//给学号（属性）赋值
		e.setAttribute("sid", stuid);
		//创建子节点
		Element e_name=doc.createElement("name");
		e_name.setTextContent(name);	
		Element e_course=doc.createElement("course");
		
		Element e_course_java=doc.createElement("java");
		e_course_java.setTextContent(java);
		Element e_course_oracle=doc.createElement("oracle");
		e_course_oracle.setTextContent(oracle);
		Element e_course_vb=doc.createElement("vb");
		e_course_vb.setTextContent(vb);
		
		//连城树
		e.appendChild(e_name);
		e.appendChild(e_course);
		e_course.appendChild(e_course_java);
		e_course.appendChild(e_course_oracle);
		e_course.appendChild(e_course_vb);
		//把新的学生添加到根元素
		doc.getDocumentElement().appendChild(e);
		
		b=true;
		return b;
	}
	
	//功能：修改学生信息
	//输入：doc,stuid
	//输出：成功与否
	public static boolean update(Document doc,String stuid) throws Exception
	{
		boolean b=false;
		//思路：重新set值即可
		//找到并输出当前学生信息
		Select(doc,stuid);
		//询问是否修改
		System.out.println("是否修改？yes/no");
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		if(br.readLine().equals("yes"))
		{
			//拿到我们关心的节点
			Element e=doc.getDocumentElement();
			Element e_name=(Element) e.getElementsByTagName("name");
			Element e_course_java=(Element) e.getElementsByTagName("java");
			Element e_course_oracle=(Element) e.getElementsByTagName("oracle");
			Element e_course_vb=(Element) e.getElementsByTagName("vb");
			//给原先节点赋值
			System.out.println("请输入学生姓名");
			String name=br.readLine();
			e_name.setTextContent(name);
			System.out.println("请输入学生java成绩");
			String java=br.readLine();
			e_course_java.setTextContent(java);
			System.out.println("请输入学生oracle成绩");
			String oracle=br.readLine();
			e_course_java.setTextContent(oracle);
			System.out.println("请输入学生vb成绩");
			String vb=br.readLine();
			e_course_java.setTextContent(vb);
			b=true;
			return b;
		}else 
		{
			return b;
		}
		
		
	}
	
	//功能：删除某个学生
	//输入：doc,stuid
	//输出：成功与否
	public static boolean delete(Document doc,String stuid)
	{
		boolean b=true;
		try {
			//根据stuid获取该学生信息
			Select(doc, stuid);
			System.out.println("是否删除该学生？Y/N");
			java.io.BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			String s=br.readLine();
			if(s.equals("Y"))
			{
				//拿到students节点
				Element students=doc.getDocumentElement();
				//拿到所有student节点
				NodeList nl=students.getElementsByTagName("student");
				//遍历，找到符合条件的节点
				for(int i=0;i<nl.getLength();i++)
				{
					//取出节点
					Element e=(Element)nl.item(i);
					//判断是否是我们要找的节点
					if(e.getAttribute("sid").equals(stuid))
					{
						//删除该节点
						e.getParentNode().removeChild(e);
					}
				}
			}else if(s.equals("N"))
			{
				return false;
			}
			
		} catch (Exception e) {
			e.printStackTrace();
			b=false;
		}
		return b;
	}
	
	//功能：更新该doc文档
	public boolean shuaxin(Document doc)
	{
		boolean b=true;
		try {
			//获取TransformerFactory 工厂对象
			TransformerFactory ff=TransformerFactory.newInstance();
			//获取Transformer
			Transformer f=ff.newTransformer();
			//用Transformer类的transform方法，实现把DOMSource（源文件），另存为StreamResult（目的地）
			f.transform(new DOMSource(doc), new StreamResult(new File("src/classes.xml")));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			b=false;
		}
		return b;
	}
	

}












