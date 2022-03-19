package com.SaxProject;

import javax.xml.parsers.*;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;
public class SaxClass {
	//ʹ��sax����ȥ����xml�ļ�.myclasses2.xml
	public static void main(String[] args) throws Exception, SAXException {
		// TODO Auto-generated method stub
		//1.����SaxParserFactory
		SAXParserFactory spf=SAXParserFactory.newInstance();
		//2.����SaxParser ������
		SAXParser saxParser=spf.newSAXParser();
		//3 ��xml�ļ����¼�����������
		saxParser.parse("src/com/SaxProject/classes.xml",new MyDefaultHandler2() );
	}
}
//�����¼�������
class MyDefaultHandler1 extends DefaultHandler{
	//�����ĵ���ʼ
	@Override
	public void startDocument() throws SAXException {
		// TODO Auto-generated method stub
		System.out.println("startDocument()");
		super.startDocument();
	}
	//����xml�ļ��е�һ��Ԫ��
	@Override
	public void startElement(String uri, String localName, String name,
			Attributes attributes) throws SAXException {
		// TODO Auto-generated method stub
		System.out.println("Ԫ������="+name);	
	}
	//����xml�ļ��е��ı�
	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		String con=new String(ch,start,length);
		//��ʾ�ı�����:
		if(!con.trim().equals("")){
			System.out.println(new String(ch,start,length));
		}
	}
	//����xml�ļ���һ��Ԫ�ؽ���</xx>
	@Override
	public void endElement(String uri, String localName, String name)
			throws SAXException {
		// TODO Auto-generated method stub
		super.endElement(uri, localName, name);
	}
	//�����ĵ�����
	@Override
	public void endDocument() throws SAXException {
		// TODO Auto-generated method stub
		System.out.println("endDocument()");
		super.endDocument();
	}
}


//��˼�������ֻ��ʾѧ�������ֺ�����
class MyDefaultHandler2 extends DefaultHandler{
	private boolean isName=false;
	private boolean isAge=false;
	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException {
		// TODO Auto-generated method stub
		String con=new String(ch,start,length);
		if(!con.trim().equals("")&&(isName||isAge)){
			System.out.println(con);
		}
		isName=false;
		isAge=false;
		//super.characters(ch, start, length);
	}
	@Override
	public void endDocument() throws SAXException {
		// TODO Auto-generated method stub
		super.endDocument();
	}
	@Override
	public void endElement(String uri, String localName, String name)
			throws SAXException {
		// TODO Auto-generated method stub
		super.endElement(uri, localName, name);
	}
	@Override
	public void startDocument() throws SAXException {
		// TODO Auto-generated method stub
		super.startDocument();
	}
	@Override
	public void startElement(String uri, String localName, String name,
			Attributes attributes) throws SAXException {
		// TODO Auto-generated method stub
		if(name.equals("����")){
			this.isName=true;
		}else if(name.equals("����")){
			this.isAge=true;
		}
	}
}


