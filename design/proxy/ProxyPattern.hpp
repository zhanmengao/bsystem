#include<iostream>
#include <string>
using namespace std;

/*
代理模式：为其他对象提供一种代理，以控制对这个对象的访问。

代练代替玩家打怪。
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
		cout << "打怪升级" << speed << "倍经验"<<endl;
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
		cout << "双倍经验打怪升级"<<endl;
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