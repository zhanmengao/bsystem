#include<iostream>
#include <string>
using namespace std;
/*
适配器模式：将一个类的接口转化成用户希望的另一个接口，是的原本由于接口不兼容而不能一起工作的那些类可以一起工作

将220V传给适配器，适配器转化为18V
*/
//转化为汉语
class Player
{
public:
	virtual void attack()
	{
		cout << "发起进攻"<<endl;
	}
	virtual void defense()
	{
		cout << "防守"<<endl;
	}
};
class Center
{
public:
	string attack()
	{
		return "attack";
	}
	string defence()
	{
		return "defence";
	}
};
class TransLater :public Player
{
public:
	TransLater(Center *center)
	{
		this->center = center;
	}
	virtual void attack()
	{
		center->attack();
		Player::attack();
	}
	virtual void defense()
	{
		center->defence();
		Player::defense();
	}
private:
	Center *center;
};



void AdapterMain()
{
	Center *center = new Center;
	TransLater translate(center);
	translate.attack();
}