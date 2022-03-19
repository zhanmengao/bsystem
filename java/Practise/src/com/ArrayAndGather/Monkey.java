/*
 * ���ܣ�1����List�ӿ�����10ֻ����
 * 2����Set�ӿ�����10ֻ���ʵ�ֲ��Һ��滻
 * 3����Map�ӿ�����10ֻ���ʵ����ɾ�Ĳ飬
 */
package com.ArrayAndGather;
import java.util.*;

public class Monkey {
	
	//����Ϊ����Ӧ��������,���䣬���
	private String num=null;
	private String name=null;
	private int age=0;
	public String getNum() {
		return num;
	}
	public void setNum(String num) {
		this.num = num;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public int getAge() {
		return age;
	}
	public void setAge(int age) {
		this.age = age;
	}

	//������Ĺ��췽��
	Monkey(String num,String name,int age)
	{
		this.num=num;
		this.name=name;
		this.age=age;
	}
	//���ӵĹ��췽��2��������ΪHashMap��key)
	Monkey(String name,int age)
	{
		this.name=name;
		this.age=age;
	}

	 //ʵ����10������
	static Monkey mk1=new Monkey("s001","aa",1);
	static Monkey mk2=new Monkey("s002","bb",2);
	static Monkey mk3=new Monkey("s003","cc",3);
	static Monkey mk4=new Monkey("s004","ee",4);
	static Monkey mk5=new Monkey("s005","ff",5);
	static Monkey mk6=new Monkey("s006","gg",6);
	static Monkey mk7=new Monkey("s007","hh",7);
	static Monkey mk8=new Monkey("s008","ii",8);
	static Monkey mk9=new Monkey("s009","jj",9);
	static Monkey mk10=new Monkey("s010","kk",10);
	public static void main(String[] args) {
		MonkeyManage.al.add(mk1);
		MonkeyManage.al.add(mk2);
		MonkeyManage.al.add(mk3);
		MonkeyManage.al.add(mk4);
		MonkeyManage.al.add(mk5);
		MonkeyManage.al.add(mk6);
		MonkeyManage.al.add(mk7);
		MonkeyManage.al.add(mk8);
		MonkeyManage.al.add(mk9);
		MonkeyManage.al.add(mk10);
		//����һ�����ӹ�����
		MonkeyManage manage=new MonkeyManage();
		manage.show_inAL();
		manage.replace_toAL("s003", "�װ�", 2);
		manage.show_inAL();
	}
}
//����һ�����ӹ�����
class MonkeyManage{
	static ArrayList<Monkey> al=new ArrayList<Monkey>();
	//ʵ������º���
	protected void add_toAL(String num,String name,int age)
	{
		Monkey temp=new Monkey(num,name,age);
		al.add(temp);
	}
	//ʵ�ֲ�ѯ����
	protected void select_inAL(String num)
	{
		for(int i=0;i<al.size();i++)
		{
			//ȡ��������Ϣ
			Monkey temp=al.get(i);
			if(num.equals(temp.getNum()))
			{
				System.out.print("�ú�����ϢΪ ����"+temp.getNum()+" ����"+temp.getName()+" ����"+temp.getAge());
			}
		}
	}
	//ʵ���޸ĺ���
	protected void replace_toAL(String num,String newname,int newage)
	{
		for(int i=0;i<al.size();i++)
		{
			Monkey temp=al.get(i);
			if(num.equals(temp.getNum()))
			{
				temp.setAge(newage);
				temp.setName(newname);
				System.out.println("�޸���ɣ�");
			}
		}
	}
	//ʵ��������к�����Ϣ
	protected void show_inAL()
	{
		for(int i=0;i<al.size();i++)
		{
			Monkey temp=al.get(i);
			System.out.println("����"+(i+1)+"��ϢΪ ����"+temp.getNum()+" ����"+temp.getName()+" ����"+temp.getAge());
		}
	}
	
	//���濪ʼ��Vector�������(�̰߳�ȫ)
	static Vector<Monkey> vt=new Vector<Monkey>();
	//ʵ����Ӻ���
	protected void add_toVT(String num,String name,int age)
	{
		Monkey temp=new Monkey(num,name,age);
		vt.add(temp);
	}
	//ʵ�ֲ�ѯ����
	protected void select_inVT(String num)
	{
		for(int i=0;i<vt.size();i++)
		{
			//ȡ��������Ϣ
			Monkey temp=vt.get(i);
			if(num.equals(temp.getNum()))
			{
				System.out.print("�ú�����ϢΪ ����"+temp.getNum()+" ����"+temp.getName()+" ����"+temp.getAge());
			}
		}
	}
	//ʵ���޸ĺ���
	protected void replace_toVT(String num,String newname,int newage)
	{
		for(int i=0;i<vt.size();i++)
		{
			Monkey temp=vt.get(i);
			if(num.equals(temp.getNum()))
			{
				temp.setAge(newage);
				temp.setName(newname);
				System.out.println("�޸���ɣ�");
			}
		}
	}
	
	//���濪ʼ��HashMap�������
	static HashMap<String,Monkey> hm=new HashMap<String, Monkey>();
	//ʵ����Ӻ���
	protected void add_toHM(String num,String name,int age)
	{
		Monkey temp=new Monkey(name,age);
		hm.put(num, temp);//������Ϊ��ֵ
	}
	//ʵ�ֲ�ѯ����
	protected void select_inHM(String num)
	{
		//������ڸ�key���򷵻ظö����Ӧ��ֵ�����򷵻�null
			Monkey temp=hm.get(num);
			if(hm!=null)
			{
				System.out.println("�ú��ӵ���Ϣ��"+temp.getNum()+temp.getName()+temp.getAge());
			}else
				System.out.println("�޸ú��ӣ�");
	}
	//ʵ���޸ĺ�����Ϣ
	protected void replace_toHM(String num,String name,int age)
	{
		Monkey temp=hm.get(num);
		if(hm!=null)
		{
			temp.setAge(age);
			temp.setName(name);
			System.out.println("�޸ĳɹ�");
		}else
			System.out.println("�޸ú��ӣ�");
	}
}














