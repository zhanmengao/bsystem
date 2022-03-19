#include<iostream>
#include <string>
using namespace std;

/*
������ģʽ����һ�����ӵĹ��������ʾ���룬ʹ��ͬ���Ĺ������̿��Դ�����ͬ��ʵ��
	���ʦ�����ù��̶�ָ�룩�����̶ӣ���Ԣ���̶ӣ��������̶ӣ����췿��
*/
class House
{
public:
	string getFloor()
	{
		return floor;
	}
	void serFloor(string floor)
	{
		this->floor = floor;
	}
	string getWall()
	{
		return wall;
	}
	void setWall(string wall)
	{
		this->wall = wall;
	}
	string getDoor()
	{
		return door;
	}
	void setDoor(string door)
	{
		this->door = door;
	}
private:
	string floor;
	string wall;
	string door;
};
class Bullider
{
	//�Ƿ��ӵ���
public:
	virtual void makeFloor() = 0;
	virtual void makeWall() = 0;
	virtual void makeDoor() = 0;
	virtual House* getHouse() = 0;
};
//���ʦ
class Director
{
public:
	void construct(Bullider *bullider)
	{
		//ָ�ӹ��̶Ӹɻ�
		bullider->makeDoor();
		bullider->makeFloor();
		bullider->makeWall();
	}
};

class bieshuBullider :public Bullider
{
public:
	bieshuBullider()
	{
		house = new House;
	}
	virtual void makeFloor()
	{
		house->serFloor("�����ĵذ�Ǻ���");
	}
	virtual void makeWall()
	{
		house->setWall("������ǽ�Ǻ���");
	}
	virtual void makeDoor()
	{
		house->setDoor("�������ŸǺ���");
	}
	virtual House* getHouse()
	{
		return house;
	}
private:
	//���һ������
	House *house;
};
class gongyuBullider :public Bullider
{
public:
	gongyuBullider()
	{
		house = new House;
	}
	virtual void makeFloor()
	{
		house->serFloor("��Ԣ�ĵذ�Ǻ���");
	}
	virtual void makeWall()
	{
		house->setWall("��Ԣ��ǽ�Ǻ���");
	}
	virtual void makeDoor()
	{
		house->setDoor("��Ԣ���ŸǺ���");
	}
	virtual House* getHouse()
	{
		return house;
	}
private:
	//���һ������
	House *house;
};

int main()
{
	Director *director = new Director;
	Bullider *bullider = new bieshuBullider;
	director->construct(bullider);
	House *house=bullider->getHouse();

	cout <<house->getDoor()<<endl;
	cout << house->getFloor() << endl;
	cout << house->getWall() << endl;

	delete bullider;
	delete director;
	return 0;
}
