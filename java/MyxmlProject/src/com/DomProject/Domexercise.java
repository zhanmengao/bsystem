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
 * ��ɶ�ѧ����xml��CRUD
 */

public class Domexercise {

	public static void main(String[] args) throws Exception{
		// TODO Auto-generated method stub
		Domexercise cc=new Domexercise();
		//���Document
		Document document=getDocument("src/com/DomProject/classes.xml");
		//���в˵�
		cc.menu(document);
		
	}
	//���ܣ����Document
	public static Document getDocument(String url)
	{
		Document doc=null;
		try {
			//��ȡdom�������Ĺ���
			DocumentBuilderFactory DBF=DocumentBuilderFactory.newInstance();
			//�õ��µ�dom����������
			DocumentBuilder DBuilder=DBF.newDocumentBuilder();
			//��DocumentBuilderȥ�����ĵ����õ�doc����
			doc=DBuilder.parse(url);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return doc;
	}
	//���ܣ����˵�
	public void menu(Document d)
	{
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
					//�õ�students
					Element e=d.getDocumentElement();
					//�û���Ҫ�鿴����ѧ��
					SelectAll(e);
				}
				else if(s.equals("view of"))
				{
					//�û���Ҫ�鿴ĳ��ѧ��
					System.out.println("��������Ҫ�鿴��ѧ��ѧ��");
					String sid=br.readLine();	
					Select(d, sid);
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
					//ִ�����
					b=add(d, sid, name, java, oracle, vb);
				}
				else if(s.equals("change"))
				{
					//�û���Ҫ����
					System.out.println("������Ҫ���ĵ�ѧ��ѧ��");
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
					//�û���Ҫɾ��
					System.out.println("����������Ҫɾ����ѧ��ѧ��");
					String sid=br.readLine();
					b=delete(d, sid);
				}
				else if(s.equals("save"))
				{
					//�û���Ҫ����
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
				System.out.println("�������!");
			}else{
				System.out.println("����ʧ�ܣ�");
			}
		}
	}
	
	//���ܣ���ɶ�dom�ı���������ӡ����ѧ����Ϣ
	//���룺ͷ���Node
	public static void SelectAll(Element n)
	{
		//������������ڵ�
		if(n.getNodeType()==n.ELEMENT_NODE && !n.getNodeName().equals("course") && !n.getNodeName().equals("students") )
		{
			//����ڵ�����ԡ�Ԫ��������������
			//����ýڵ���ѧ�����ԣ�student�ڵ㣩
			if(!n.getAttribute("sid").equals(""))
			{
				System.out.println();
				System.out.println("ѧ��ѧ��"+n.getAttribute("sid")+"\t\t");
			}
			else
			{
				//�������ǩText
				System.out.print(n.getNodeName()+"Ϊ"+n.getTextContent()+"\t\t");
			}	
		}
		//��ȡ��n���ӽڵ�
		NodeList nl=n.getChildNodes();
		//���ӽڵ���ִ����ͬ�Ĳ���
		for(int i=0;i<nl.getLength();i++)
		{
			//ȡ���ӽڵ�
			if(nl.item(i).getNodeType()==Node.ELEMENT_NODE)
			{
				Element nc=(Element) nl.item(i);
				//���ӽڵ��ٴ�ȡ�������ӽڵ�
				SelectAll(nc);
			}
		}
	}
	
	public static void Select(Document doc,String StuId)
	{
		//����ѧ���ڵ� �ҵ�����ѧ��
		NodeList nl=doc.getElementsByTagName("student");
		//ȡ������ѧ������ƥ������
		for(int i=0;i<nl.getLength();i++)
		{
			//ȡ����ѧ��
			Element e=(Element)nl.item(i);
			//ƥ��
			if(e.getAttribute("sid").equals(StuId))
			{
				//ƥ����� �����Ϣ
				SelectAll(e);
			}
		}	
	}
	
	//���ܣ����һ��ѧ��
	//���룺doc,stuid,name,java,oracle,vb
	//������ɹ����
	public static boolean add(org.w3c.dom.Document doc,String stuid,String name,String java,String oracle,String vb)
	{
		boolean b=false;
		//����һ���µ�ѧ���ڵ�
		Element e=doc.createElement("student");
		//��ѧ�ţ����ԣ���ֵ
		e.setAttribute("sid", stuid);
		//�����ӽڵ�
		Element e_name=doc.createElement("name");
		e_name.setTextContent(name);	
		Element e_course=doc.createElement("course");
		
		Element e_course_java=doc.createElement("java");
		e_course_java.setTextContent(java);
		Element e_course_oracle=doc.createElement("oracle");
		e_course_oracle.setTextContent(oracle);
		Element e_course_vb=doc.createElement("vb");
		e_course_vb.setTextContent(vb);
		
		//������
		e.appendChild(e_name);
		e.appendChild(e_course);
		e_course.appendChild(e_course_java);
		e_course.appendChild(e_course_oracle);
		e_course.appendChild(e_course_vb);
		//���µ�ѧ����ӵ���Ԫ��
		doc.getDocumentElement().appendChild(e);
		
		b=true;
		return b;
	}
	
	//���ܣ��޸�ѧ����Ϣ
	//���룺doc,stuid
	//������ɹ����
	public static boolean update(Document doc,String stuid) throws Exception
	{
		boolean b=false;
		//˼·������setֵ����
		//�ҵ��������ǰѧ����Ϣ
		Select(doc,stuid);
		//ѯ���Ƿ��޸�
		System.out.println("�Ƿ��޸ģ�yes/no");
		BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
		if(br.readLine().equals("yes"))
		{
			//�õ����ǹ��ĵĽڵ�
			Element e=doc.getDocumentElement();
			Element e_name=(Element) e.getElementsByTagName("name");
			Element e_course_java=(Element) e.getElementsByTagName("java");
			Element e_course_oracle=(Element) e.getElementsByTagName("oracle");
			Element e_course_vb=(Element) e.getElementsByTagName("vb");
			//��ԭ�Ƚڵ㸳ֵ
			System.out.println("������ѧ������");
			String name=br.readLine();
			e_name.setTextContent(name);
			System.out.println("������ѧ��java�ɼ�");
			String java=br.readLine();
			e_course_java.setTextContent(java);
			System.out.println("������ѧ��oracle�ɼ�");
			String oracle=br.readLine();
			e_course_java.setTextContent(oracle);
			System.out.println("������ѧ��vb�ɼ�");
			String vb=br.readLine();
			e_course_java.setTextContent(vb);
			b=true;
			return b;
		}else 
		{
			return b;
		}
		
		
	}
	
	//���ܣ�ɾ��ĳ��ѧ��
	//���룺doc,stuid
	//������ɹ����
	public static boolean delete(Document doc,String stuid)
	{
		boolean b=true;
		try {
			//����stuid��ȡ��ѧ����Ϣ
			Select(doc, stuid);
			System.out.println("�Ƿ�ɾ����ѧ����Y/N");
			java.io.BufferedReader br=new BufferedReader(new InputStreamReader(System.in));
			String s=br.readLine();
			if(s.equals("Y"))
			{
				//�õ�students�ڵ�
				Element students=doc.getDocumentElement();
				//�õ�����student�ڵ�
				NodeList nl=students.getElementsByTagName("student");
				//�������ҵ����������Ľڵ�
				for(int i=0;i<nl.getLength();i++)
				{
					//ȡ���ڵ�
					Element e=(Element)nl.item(i);
					//�ж��Ƿ�������Ҫ�ҵĽڵ�
					if(e.getAttribute("sid").equals(stuid))
					{
						//ɾ���ýڵ�
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
	
	//���ܣ����¸�doc�ĵ�
	public boolean shuaxin(Document doc)
	{
		boolean b=true;
		try {
			//��ȡTransformerFactory ��������
			TransformerFactory ff=TransformerFactory.newInstance();
			//��ȡTransformer
			Transformer f=ff.newTransformer();
			//��Transformer���transform������ʵ�ְ�DOMSource��Դ�ļ��������ΪStreamResult��Ŀ�ĵأ�
			f.transform(new DOMSource(doc), new StreamResult(new File("src/classes.xml")));
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			b=false;
		}
		return b;
	}
	

}












