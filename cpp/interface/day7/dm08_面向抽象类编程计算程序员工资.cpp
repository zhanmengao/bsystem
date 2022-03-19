
#include <iostream>
using namespace std;

/*
��дһ��C++����, �������Ա( programmer )����
	1 Ҫ���ܼ������������Ա( junior_programmer ) �м�����Ա ( mid_programmer )�߼�����Ա( adv_programmer)�Ĺ���
	2 Ҫ�����ó�����ͳһ����,����������չ, ����:����, ���� �ܹ�ʦ (architect ) �Ĺ���
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


//�ܹ�ʦ
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


//���㺯�� �򵥵Ŀ��
void CalProgSal(programmer *base)
{
	base->getSal();
}

void main801()
{
	junior_programmer jp("С��", "����", 4000);

	mid_programmer mp("С��", "�м�", 8600);
	adv_programmer ap("С��", "�߼�", 15000);

	//ϵͳ��չ
	architect ar("���Ǹ�ˮƽѧԱ", "�ܹ�ʦ", 24000);

	CalProgSal(&jp);
	CalProgSal(&mp);
	CalProgSal(&ap);
	CalProgSal(&ar);
}