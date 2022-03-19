
#include <iostream>
using namespace std;
#include "string"

class House
{
public:
	void setDoor(string door)
	{
		this->m_door = door;
	}

	void setWall(string wall)
	{
		this->m_wall = wall;
	}
	void setWindow(string window)
	{
		this->m_window = window;
	}

	//--
	string getDoor( )
	{
		cout << m_door << endl;
		return this->m_door ;
	}

	string getWall()
	{
		cout << m_wall << endl;
		return this->m_wall;
	}
	string getWindow()
	{
		cout << m_window << endl;
		return m_window;
	}

private:
	string	m_door;
	string	m_wall;
	string	m_window;
};


class Build
{
public:
	Build()
	{
		m_house = new House;
	}

	void makeBuild()
	{
		buildDoor(m_house);
		buildWall(m_house);
		buildWindow(m_house);
	}
	void buildDoor(House *h)
	{
		h->setDoor("��");
	}
	void buildWall(House *h)
	{
		h->setWall("ǽ");
	}
	void buildWindow(House *h)
	{
		h->setWindow("��");
	}

	House *getHouse()
	{
		return m_house;
	}

private:
	House *m_house;

};

void main()
{
	/* �ͻ�ֱ���췿��
	House *house = new House;
	house->setDoor("��");
	house->setWall("ǽ��");
	house->setWindow("����");
	delete house;
	*/

	//�빤�̶� ���췿��
	House *house = NULL;
	Build * build = new Build;
	build->makeBuild();
	house = build->getHouse();
	house->getDoor();
	house->getWall();
	house->getWindow();

	cout<<"hello..."<<endl;
	system("pause");
	return ;
}