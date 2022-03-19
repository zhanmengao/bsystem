
#include <iostream>
using namespace std;

/*
编写一个C++程序, 计算程序员( programmer )工资
	1 要求能计算出初级程序员( junior_programmer ) 中级程序员 ( mid_programmer )高级程序员( adv_programmer)的工资
	2 要求利用抽象类统一界面,方便程序的扩展, 比如:新增, 计算 架构师 (architect ) 的工资
*/


class programmer
{
public:
	virtual void getSal() = 0;
};


class junior_programmer : public programmer
{
public:
	junior_programmer(char *name, char *job, int sal)
	{
		this->name = name;
		this->job = job;
		this->sal = sal;
	}
	virtual void getSal()
	{
		cout<<name<<" " << job << " : " << sal <<endl;
	}
private:
	char *name;
	char *job;
	int sal;
};

//zhong 
class mid_programmer : public programmer
{
public:
	mid_programmer(char *name, char *job, int sal)
	{
		this->name = name;
		this->job = job;
		this->sal = sal;
	}
	virtual void getSal()
	{
		cout<<name<<" " << job << " : " << sal <<endl;
	}
private:
	char *name;
	char *job;
	int sal;
};

//adv_programmer
class adv_programmer : public programmer
{
public:
	adv_programmer(char *name, char *job, int sal)
	{
		this->name = name;
		this->job = job;
		this->sal = sal;
	}
	virtual void getSal()
	{
		cout<<name<<" " << job << " : " << sal <<endl;
	}
private:
	char *name;
	char *job;
	int sal;
};


//架构师
class architect : public programmer 
{
public:
	architect(char *name, char *job, int sal)
	{
		this->name = name;
		this->job = job;
		this->sal = sal;
	}
	virtual void getSal()
	{
		cout<<name<<" " << job << " : " << sal <<endl;
	}
private:
	char *name;
	char *job;
	int sal;
};


//计算函数 简单的框架
void CalProgSal(programmer *base)
{
	base->getSal();
}

void main801()
{
	junior_programmer jp("小王", "初级", 4000);

	mid_programmer mp("小张", "中级", 8600);
	adv_programmer ap("小李", "高级", 15000);

	//系统扩展
	architect ar("传智高水平学员", "架构师", 24000);

	CalProgSal(&jp);
	CalProgSal(&mp);
	CalProgSal(&ap);
	CalProgSal(&ar);
}