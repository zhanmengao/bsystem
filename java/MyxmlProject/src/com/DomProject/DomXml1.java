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
	//ʹ��dom������ xml�ļ�����crud����
	public static void main(String[] args) throws Exception{

		//����һ��DocumentBuilderFactory
		DocumentBuilderFactory dbf=DocumentBuilderFactory.newInstance();
		//ͨ��DocumentBuilderFactory,�õ�һ��DocumentBuilder����
		DocumentBuilder dBuilder=dbf.newDocumentBuilder();
		//ָ�������ĸ�xml�ļ�
		Document document=dBuilder.parse("src/classes.xml");
		//System.out.println(document);???
		//list(document);
		//read(document);
		//add(document);
		upd(document);
		System.out.println("ok");
	}
	//����Ԫ��(�ѵ�һ��ѧ�������ĳ� �ν�)
	public static void upd(Document doc) throws Exception{
			//�ҵ�
			Element node=(Element) doc.getElementsByTagName("ѧ��").item(0);
			Element node_name=(Element) node.getElementsByTagName("����").item(0);
			node_name.setTextContent("�ν�");
			//node_name.setAttribute("sex", arg1)
			//����xml
			//�õ�TransformerFactory
			TransformerFactory tff=TransformerFactory.newInstance();
			//ͨ��TransformerFactory �õ�һ��ת����
			Transformer tf=tff.newTransformer();
			tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	//ɾ��һ��Ԫ��(ɾ��С��2ѧ��)
	public static void del(Document doc) throws Exception{
		//����Ҫ�ҵ����ѧ��
		//Node node= doc.getElementsByTagName("ѧ��").item(0);
		//node.getParentNode().removeChild(node);
		//ɾ��ѧ����sex����
		Element node= (Element) doc.getElementsByTagName("ѧ��").item(0);
		node.removeAttribute("sex");
		//����xml
		//�õ�TransformerFactory
		TransformerFactory tff=TransformerFactory.newInstance();
		//ͨ��TransformerFactory �õ�һ��ת����
		Transformer tf=tff.newTransformer();
		tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	//���һ��ѧ����xml�ļ���
	public static void add(Document doc) throws Exception{
		//����һ���µ�ѧ���ڵ�
		Element newStu=doc.createElement("ѧ��");
		//���һ������ֵ
		newStu.setAttribute("sex", "��");
		Element newStu_name=doc.createElement("����");
		newStu_name.setTextContent("С��2");
		Element newStu_age=doc.createElement("����");
		newStu_age.setTextContent("34");
		Element newStu_intro=doc.createElement("����");
		newStu_intro.setTextContent("����һ���ú���");
		newStu.appendChild(newStu_name);
		newStu.appendChild(newStu_age);
		newStu.appendChild(newStu_intro);
		
		//���µ�ѧ���ڵ���ӵ���Ԫ��
		doc.getDocumentElement().appendChild(newStu);
		//�õ�TransformerFactory
		TransformerFactory tff=TransformerFactory.newInstance();
		//ͨ��TransformerFactory �õ�һ��ת����
		Transformer tf=tff.newTransformer();
		tf.transform(new DOMSource(doc), new StreamResult("src/classes.xml"));
	}
	
	//����Ĳ�ѯĳ��ѧ������Ϣ(��ʾ��һ��ѧ����������Ϣ)
	//�뿼����λ�ȡĳ��Ԫ�ص�����ֵ,(ȡ��)
	public static void read(Document doc){
		NodeList nl=doc.getElementsByTagName("ѧ��");
		//ȡ����һ��ѧ��
		Element stu=(Element) nl.item(0);
		System.out.println("ѧ�����Ա���"+stu.getAttribute("sex"));
		Element name=(Element) stu.getElementsByTagName("����").item(0);
		System.out.println(name.getTextContent());
		//System.out.println("����"+nl.getLength());
		//xpath
	}

	//������xml�ļ�
	public static void list(Node node){
		if(node.getNodeType()==node.ELEMENT_NODE){
			System.out.println("����"+node.getNodeName());
		}
		//ȡ��node���ӽڵ�
		NodeList nodeList=node.getChildNodes();
		for(int i=0;i<nodeList.getLength();i++){
			//��ȥ��ʾ
			Node n=nodeList.item(i);
			list(n);
		}
	}
}
