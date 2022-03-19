package com.DomProject;
import javax.xml.parsers.*;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
public class DomXml1 {
	//使用dom技术对 xml文件进行crud操作
	public static void main(String[] args) throws Exception{

		//创建一个DocumentBuilderFactory
		DocumentBuilderFactory dbf=DocumentBuilderFactory.newInstance();
		//通过DocumentBuilderFactory,得到一个DocumentBuilder对象
		DocumentBuilder dBuilder=dbf.newDocumentBuilder();
		//指定解析哪个xml文件
		Document document=dBuilder.parse("src/classes.xml");
		//System.out.println(document);???
		//list(document);
		//read(document);
		//add(document);
		upd(document);
		System.out.println("ok");
	}
	//更新元素(把第一个学生的名改成 宋江)
	public static void upd(Document doc) throws Exception{
			//找到
			Element node=(Element) doc.getElementsByTagName("学生").item(0);
			Element node_name=(Element) node.getElementsByTagName("名字").item(0);
			node_name.setTextContent("宋江");
			//node_name.setAttribute("sex", arg1)
			//更新xml
			//得到TransformerFactory
			TransformerFactory tff=TransformerFactory.newInstance();
			//通过TransformerFactory 得到一个转换器
			Transformer tf=tff.newTransformer();
			tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	//删除一个元素(删除小明2学生)
	public static void del(Document doc) throws Exception{
		//首先要找到这个学生
		//Node node= doc.getElementsByTagName("学生").item(0);
		//node.getParentNode().removeChild(node);
		//删除学生的sex属性
		Element node= (Element) doc.getElementsByTagName("学生").item(0);
		node.removeAttribute("sex");
		//更新xml
		//得到TransformerFactory
		TransformerFactory tff=TransformerFactory.newInstance();
		//通过TransformerFactory 得到一个转换器
		Transformer tf=tff.newTransformer();
		tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	//添加一个学生到xml文件中
	public static void add(Document doc) throws Exception{
		//创建一个新的学生节点
		Element newStu=doc.createElement("学生");
		//添加一个属性值
		newStu.setAttribute("sex", "男");
		Element newStu_name=doc.createElement("名字");
		newStu_name.setTextContent("小明2");
		Element newStu_age=doc.createElement("年龄");
		newStu_age.setTextContent("34");
		Element newStu_intro=doc.createElement("介绍");
		newStu_intro.setTextContent("这是一个好孩子");
		newStu.appendChild(newStu_name);
		newStu.appendChild(newStu_age);
		newStu.appendChild(newStu_intro);
		
		//把新的学生节点添加到根元素
		doc.getDocumentElement().appendChild(newStu);
		//得到TransformerFactory
		TransformerFactory tff=TransformerFactory.newInstance();
		//通过TransformerFactory 得到一个转换器
		Transformer tf=tff.newTransformer();
		tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	
	//具体的查询某个学生的信息(显示第一个学生的所有信息)
	//请考虑如何获取某个元素的属性值,(取出)
	public static void read(Document doc){
		NodeList nl=doc.getElementsByTagName("学生");
		//取出第一个学生
		Element stu=(Element) nl.item(0);
		System.out.println("学生的性别是"+stu.getAttribute("sex"));
		Element name=(Element) stu.getElementsByTagName("年龄").item(0);
		System.out.println(name.getTextContent());
		//System.out.println("发现"+nl.getLength());
		//xpath
	}

	//遍历该xml文件
	public static void list(Node node){
		if(node.getNodeType()==node.ELEMENT_NODE){
			System.out.println("名字"+node.getNodeName());
		}
		//取出node的子节点
		NodeList nodeList=node.getChildNodes();
		for(int i=0;i<nodeList.getLength();i++){
			//再去显示
			Node n=nodeList.item(i);
			list(n);
		}
	}
}
