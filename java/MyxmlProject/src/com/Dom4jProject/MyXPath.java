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
	//ȡ����
	public static void select(Element root)
	{
		//������XPath�﷨��������һ���ļ�ϵͳ�ж�λ�ļ�,
		//���·����б�� / ��ʼ, ��ô��·���ͱ�ʾ��һ��Ԫ�صľ���·��
		List list=root.selectNodes("/AAA");
	
		//���·����˫б�� // ��ͷ, ���ʾѡ���ĵ�����������˫б��//֮������Ԫ��(���۲㼶��ϵ)
		list=root.selectNodes("//AAA");
		
		//�Ǻ� * ��ʾѡ���������Ǻ�֮ǰ��·������λ��Ԫ��
		list=root.selectNodes("/AAA/CCC/DDD/*");
		
		//�������ı��ʽ���Խ�һ����ָ��Ԫ��, �������ֱ�ʾԪ����ѡ�����λ��, ��last()�������ʾѡ���е����һ��Ԫ��.
		list=root.selectNodes("/AAA/BBB[1]");
		
		//ѡ��������id����ֵΪ'b1'��BBBԪ��
		list=root.selectNodes("//BBB[@id='b1']");
		
		//count()�������Լ�����ѡԪ�صĸ���
		list=root.selectNodes("//*[count(BBB)=2]");//ѡ����2��BBB��Ԫ�ص�Ԫ��
		
		//name()��������Ԫ�ص�����, 
		//start-with()�����ڸú����ĵ�һ�������ַ������Եڶ��������ַ���ʼ���������true, 
		//contains()���������һ���ַ������������еڶ����ַ�������ʱ����true.
		list=root.selectNodes("//*[name()='BBB']");
		list=root.selectNodes("//*[starts-with(name(),'B')]");//ѡ������������"B"��ʼ��Ԫ��
		list=root.selectNodes("//*[contains(name(),'C')]");//ѡ���������ư���"C"��Ԫ��
		
		System.out.println(list.size());
		
	}
}
