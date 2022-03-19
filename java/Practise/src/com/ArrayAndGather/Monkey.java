/*
 * 功能：1、用List接口容纳10只猴子
 * 2、用Set接口容纳10只猴，并实现查找和替换
 * 3、用Map接口容纳10只猴，并实现增删改查，
 */
package com.ArrayAndGather;
import java.util.*;

public class Monkey {
	
	//我认为猴子应该有名字,年龄，编号
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

	//猴子类的构造方法
	Monkey(String num,String name,int age)
	{
		this.num=num;
		this.name=name;
		this.age=age;
	}
	//猴子的构造方法2（工号作为HashMap的key)
	Monkey(String name,int age)
	{
		this.name=name;
		this.age=age;
	}

	 //实例化10个猴子
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
		//创建一个猴子管理者
		MonkeyManage manage=new MonkeyManage();
		manage.show_inAL();
		manage.replace_toAL("s003", "白白", 2);
		manage.show_inAL();
	}
}
//定义一个猴子管理类
class MonkeyManage{
	static ArrayList<Monkey> al=new ArrayList<Monkey>();
	//实现添加新猴子
	protected void add_toAL(String num,String name,int age)
	{
		Monkey temp=new Monkey(num,name,age);
		al.add(temp);
	}
	//实现查询猴子
	protected void select_inAL(String num)
	{
		for(int i=0;i<al.size();i++)
		{
			//取出猴子信息
			Monkey temp=al.get(i);
			if(num.equals(temp.getNum()))
			{
				System.out.print("该猴子信息为 工号"+temp.getNum()+" 姓名"+temp.getName()+" 年龄"+temp.getAge());
			}
		}
	}
	//实现修改猴子
	protected void replace_toAL(String num,String newname,int newage)
	{
		for(int i=0;i<al.size();i++)
		{
			Monkey temp=al.get(i);
			if(num.equals(temp.getNum()))
			{
				temp.setAge(newage);
				temp.setName(newname);
				System.out.println("修改完成！");
			}
		}
	}
	//实现输出所有猴子信息
	protected void show_inAL()
	{
		for(int i=0;i<al.size();i++)
		{
			Monkey temp=al.get(i);
			System.out.println("猴子"+(i+1)+"信息为 工号"+temp.getNum()+" 姓名"+temp.getName()+" 年龄"+temp.getAge());
		}
	}
	
	//下面开始用Vector处理猴子(线程安全)
	static Vector<Monkey> vt=new Vector<Monkey>();
	//实现添加猴子
	protected void add_toVT(String num,String name,int age)
	{
		Monkey temp=new Monkey(num,name,age);
		vt.add(temp);
	}
	//实现查询猴子
	protected void select_inVT(String num)
	{
		for(int i=0;i<vt.size();i++)
		{
			//取出猴子信息
			Monkey temp=vt.get(i);
			if(num.equals(temp.getNum()))
			{
				System.out.print("该猴子信息为 工号"+temp.getNum()+" 姓名"+temp.getName()+" 年龄"+temp.getAge());
			}
		}
	}
	//实现修改猴子
	protected void replace_toVT(String num,String newname,int newage)
	{
		for(int i=0;i<vt.size();i++)
		{
			Monkey temp=vt.get(i);
			if(num.equals(temp.getNum()))
			{
				temp.setAge(newage);
				temp.setName(newname);
				System.out.println("修改完成！");
			}
		}
	}
	
	//下面开始用HashMap处理猴子
	static HashMap<String,Monkey> hm=new HashMap<String, Monkey>();
	//实现添加猴子
	protected void add_toHM(String num,String name,int age)
	{
		Monkey temp=new Monkey(name,age);
		hm.put(num, temp);//工号作为键值
	}
	//实现查询猴子
	protected void select_inHM(String num)
	{
		//如果存在该key，则返回该对象对应的值，否则返回null
			Monkey temp=hm.get(num);
			if(hm!=null)
			{
				System.out.println("该猴子的信息是"+temp.getNum()+temp.getName()+temp.getAge());
			}else
				System.out.println("无该猴子！");
	}
	//实现修改猴子信息
	protected void replace_toHM(String num,String name,int age)
	{
		Monkey temp=hm.get(num);
		if(hm!=null)
		{
			temp.setAge(age);
			temp.setName(name);
			System.out.println("修改成功");
		}else
			System.out.println("无该猴子！");
	}
}














