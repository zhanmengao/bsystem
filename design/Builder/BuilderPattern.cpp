#include<iostream>
#include <string>
using namespace std;

/*
建造者模式：将一个复杂的构建与其表示分离，使得同样的构建过程可以创建不同的实例
	设计师（调用工程队指针），工程队（公寓工程队，别墅工程队），造房子
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
	//盖房子的人
public:
	virtual void makeFloor() = 0;
	virtual void makeWall() = 0;
	virtual void makeDoor() = 0;
	virtual House* getHouse() = 0;
};
//设计师
class Director
{
public:
	void construct(Bullider *bullider)
	{
		//指挥工程队干活
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
		house->serFloor("别墅的地板盖好了");
	}
	virtual void makeWall()
	{
		house->setWall("别墅的墙盖好了");
	}
	virtual void makeDoor()
	{
		house->setDoor("别墅的门盖好了");
	}
	virtual House* getHouse()
	{
		return house;
	}
private:
	//组合一个房子
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
		house->serFloor("公寓的地板盖好了");
	}
	virtual void makeWall()
	{
		house->setWall("公寓的墙盖好了");
	}
	virtual void makeDoor()
	{
		house->setDoor("公寓的门盖好了");
	}
	virtual House* getHouse()
	{
		return house;
	}
private:
	//组合一个房子
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
