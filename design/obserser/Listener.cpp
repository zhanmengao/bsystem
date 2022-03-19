/*
观察者模式 / 监视者模式

定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。

适用于：
定义对象间一种一对多的依赖关系，使得每一个对象改变状态，则所有依赖于他们的对象都会得到通知。

使用场景：定义了一种一对多的关系，让多个观察对象（公司员工）同时监听一个主题对象（秘书），主题对象状态发生变化时，会通知所有的观察者，使它们能够更新自己。

多个公司员工 监听秘书 由秘书通知员工老板来了
*/
#include<string>
#include<iostream>
#include<list>
using namespace std;
//订阅者
class SubscribeInterface
{
public:
	virtual void OnRecv(const char* action) = 0;
};

class Subscribe :public SubscribeInterface
{
public:
	Subscribe()
	{
	}
	Subscribe(const char* name)
	{
		m_name = name;
	}
	virtual void OnRecv(const char* action)
	{
		cout << m_name << "	听到了	" << action << std::endl;
	}
private:
	std::string		m_name;
};

//发布者
class Publisher
{
public:
	void Registered(SubscribeInterface* sub)
	{
		mSubscribeList.push_back(sub);
	}
	void DelSubscribe(SubscribeInterface* sub)
	{
		mSubscribeList.remove(sub);
	}
	void Notify(const char* msg)
	{
		for (auto it = mSubscribeList.begin(); it != mSubscribeList.end(); it++)
		{
			(*it)->OnRecv(msg);
		}
	}
private:
	list<SubscribeInterface*> mSubscribeList;
};

int main()
{
	Subscribe* s1 = new Subscribe("鸣人");
	Subscribe* s2 = new Subscribe("鼬");
	Subscribe* s3 = new Subscribe("卡卡西");
	Subscribe* s4 = new Subscribe("自来也");

	Publisher pub;
	pub.Registered(s1);
	pub.Registered(s2);
	pub.Registered(s3);
	pub.Registered(s4);
	pub.Notify("人们互相理解的时代终将到来");
	return 0;
}