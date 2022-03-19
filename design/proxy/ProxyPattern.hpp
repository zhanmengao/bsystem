#include<iostream>
#include <string>
using namespace std;

/*
����ģʽ��Ϊ���������ṩһ�ִ����Կ��ƶ��������ķ��ʡ�

����������Ҵ�֡�
*/
class Subject
{
public:
	virtual void atk() = 0;
};
class XiaoyaoLi : public Subject
{
public:
	virtual void atk()
	{
		cout << "�������" << speed << "������"<<endl;
	}
private:
	float speed = 1;
};

class daiLian :public Subject
{
public:
	daiLian(XiaoyaoLi li)
	{
		this->li = li;
	}
	virtual void atk()
	{
		li.atk();
		cout << "˫������������"<<endl;
	}
private:
	XiaoyaoLi li;
};
void ProxyMain()
{
	XiaoyaoLi *li = new XiaoyaoLi;
	Subject *sub = new daiLian(*li);
	sub->atk();

	delete li;
	delete sub;
}