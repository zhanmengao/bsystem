package com.SaxProject;

import java.io.File;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/*
 * ���ܣ�ʵ��SAX��ѧ����ѯϵͳ
 */
public class Saxexercise {
	public static void main(String []args)
	{
		Saxexercise sa=new Saxexercise();
		sa.start("src/classes.xml", new MyDefaultHander_select("003"));
	}
	//���ܣ�����xml�ļ����¼��������
	public void start(String url,DefaultHandler myDefaultHandler)
	{
		try {
			SAXParserFactory factory=SAXParserFactory.newInstance();
			SAXParser saxparser=factory.newSAXParser();	
			saxparser.parse(new File(url),myDefaultHandler);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

//�¼�������
class MyDefaultHander extends DefaultHandler{
	boolean IsName=false;
	boolean IsJava=false;
	boolean IsOracle=false;
	boolean IsVb=false;
	@Override
	public void startDocument() throws SAXException {
		//��ʼ���ĵ�
	}

	@Override
	public void endDocument() throws SAXException {
		
	}
	//��ǩ��ʼ
	public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
			
		//�����ѧ����ǩ�����������
		if(qName.equals("student"))
		{
			System.out.println();
			System.out.print("ѧ��"+attributes.getValue("sid")+"\t\t");
		}
		else if(qName.equals("name"))
		{
			IsName=true;
		}
		else if(qName.equals("java"))
		{
			IsJava=true;
		}
		else if(qName.equals("oracle"))
		{
			IsOracle=true;
		}
		else if(qName.equals("vb"))
		{
			IsVb=true;
		}
	}

	@Override
	public void endElement(String uri, String localName, String qName) throws SAXException {

	}

	@Override
	//�����ı�ʱʹ��
	public void characters(char[] ch, int start, int length) throws SAXException {
		String text=new String(ch, start, length);
		if(IsName==true)
		{
			System.out.print("����"+text+"\t\t");
			IsName=false;
		}
		else if(IsJava==true)
		{
			System.out.print("Java�ɼ�"+text+"\t");
			IsJava=false;
		}
		else if(IsOracle==true)
		{
			System.out.print("Oracle�ɼ�"+text+"\t");
			IsOracle=false;
		}
		else if(IsVb==true)
		{
			System.out.print("Vb�ɼ�"+text+"\t");
			IsVb=false;
		}
	}
}

class MyDefaultHander_select extends DefaultHandler{
	private String SelectId=""; 
	boolean IsMyID=false;
	boolean IsName=false;
	boolean IsJava=false;
	boolean IsOracle=false;
	boolean IsVb=false;
	//�����ʱ��Ҫ�ṩ��ѯ���˵�ID
	public MyDefaultHander_select(String SelectId)
	{
		this.SelectId=SelectId;
	}
	public String getSelectId() {
		return SelectId;
	}

	public void setSelectId(String selectId) {
		SelectId = selectId;
	}
	@Override
	public void startDocument() throws SAXException {
		//��ʼ���ĵ�
	}

	@Override
	public void endDocument() throws SAXException {
		
	}

	
	//��ǩ��ʼ
	public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
		if(attributes.getValue(0)!=null)
		{
			if(attributes.getValue("sid").equals(SelectId))
			{
				IsMyID=true;
			}
		}
		//�����ѧ����ǩ�����������
		if(qName.equals("student") && IsMyID==true)
		{
			System.out.println();
			System.out.print("ѧ��"+attributes.getValue("sid")+"\t\t");
		}
		else if(qName.equals("name")&& IsMyID==true)
		{
			IsName=true;
		}
		else if(qName.equals("java")&& IsMyID==true)
		{
			IsJava=true;
		}
		else if(qName.equals("oracle")&& IsMyID==true)
		{
			IsOracle=true;
		}
		else if(qName.equals("vb")&& IsMyID==true)
		{
			IsVb=true;
		}
		

	}

	@Override
	public void endElement(String uri, String localName, String qName) throws SAXException {
		//���ѧ����ǩ���� ���޸�IsMyId
		if(qName.equals("student"))
		{
			IsMyID=false;
		}
	}

	@Override
	//�����ı�ʱʹ��
	public void characters(char[] ch, int start, int length) throws SAXException {
		String text=new String(ch, start, length);
		if(IsName==true)
		{
			System.out.print("����"+text+"\t\t");
			IsName=false;
		}
		else if(IsJava==true)
		{
			System.out.print("Java�ɼ�"+text+"\t");
			IsJava=false;
		}
		else if(IsOracle==true)
		{
			System.out.print("Oracle�ɼ�"+text+"\t");
			IsOracle=false;
		}
		else if(IsVb==true)
		{
			System.out.print("Vb�ɼ�"+text+"\t");
			IsVb=false;
		}
	}
}



