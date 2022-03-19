
#include <iostream>
using namespace std;
#include "string"
#include "list"


class Secretary;


//�۲��� 
class PlayserObserver
{
public:
	PlayserObserver(Secretary *secretary)
	{
		this->m_secretary = secretary;
	}
	void update(string action)
	{
		cout << "action:" << action << endl;
		cout << "�ϰ����� �Һܺ��°�..." << endl;
		
	}
private:
	Secretary *m_secretary;
};
//
class Secretary
{
public:
	Secretary()
	{
		m_list.clear();
	}
	void Notify(string info)
	{
		//�����е� �۲��� ���� �鱨
		for ( list<PlayserObserver *>::iterator it=m_list.begin(); it!=m_list.end(); it++)
		{
			(*it)->update(info); //
		}
	}

	void setPlayserObserver(PlayserObserver *o)
	{
		m_list.push_back(o);
	}

private:
	list<PlayserObserver *> m_list;
};
void main()
{
	Secretary			*secretary = NULL;
	PlayserObserver		*po1 = NULL;
	PlayserObserver		*po2 = NULL;

	secretary = new Secretary;
	po1 = new PlayserObserver(secretary);
	po2 = new PlayserObserver(secretary);

	secretary->setPlayserObserver(po1);
	secretary->setPlayserObserver(po2);

	secretary->Notify("�ϰ�����") ;
	secretary->Notify("�ϰ�����");
	delete secretary ;
	delete po1 ;
	delete po2 ;

	system("pause");
	return ;
}