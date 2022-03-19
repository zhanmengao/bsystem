/*
�۲���ģʽ / ������ģʽ

���������һ��һ�Զ��������ϵ����һ�������״̬�����ı�ʱ���������������Ķ��󶼵õ�֪ͨ�����Զ����¡�

�����ڣ�
��������һ��һ�Զ��������ϵ��ʹ��ÿһ������ı�״̬�����������������ǵĶ��󶼻�õ�֪ͨ��

ʹ�ó�����������һ��һ�Զ�Ĺ�ϵ���ö���۲���󣨹�˾Ա����ͬʱ����һ������������飩���������״̬�����仯ʱ����֪ͨ���еĹ۲��ߣ�ʹ�����ܹ������Լ���

�����˾Ա�� �������� ������֪ͨԱ���ϰ�����
*/
#include<string>
#include<iostream>
#include<list>
using namespace std;
//������
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
		cout << m_name << "	������	" << action << std::endl;
	}
private:
	std::string		m_name;
};

//������
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
	Subscribe* s1 = new Subscribe("����");
	Subscribe* s2 = new Subscribe("��");
	Subscribe* s3 = new Subscribe("������");
	Subscribe* s4 = new Subscribe("����Ҳ");

	Publisher pub;
	pub.Registered(s1);
	pub.Registered(s2);
	pub.Registered(s3);
	pub.Registered(s4);
	pub.Notify("���ǻ�������ʱ���ս�����");
	return 0;
}