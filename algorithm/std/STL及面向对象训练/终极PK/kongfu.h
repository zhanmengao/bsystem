#include<string>
#include <iostream>

using namespace std;

//战损描述
class Damage
{
public:
	Damage(double value = 0, int mode = Damage::Blood, bool isSustain = false, int round = 0)
		:_value(value), _mode(mode), _isSustained(isSustain), _round(round)
	{}
	enum MyEnum
	{
		Blood,
		Persont
	};
	double getValue(){ return _value; }	//战损值
	int getMode(){ return _mode; }	//战损模式  血量值/百分比
	bool isSustained(){ return _isSustained; } //是否有持续效果
	int sustainedRound(){ return _round; } //持续效果回合

private:
	double _value;
	int _mode;
	bool _isSustained;
	int _round;
};

//招式接口
class IKongFu
{
public:
	virtual const string& getName() = 0;
	virtual Damage& getDamage() = 0;
};


//招式基类
class KongfuBase : public IKongFu
{
public:
	KongfuBase() :_name("--"){}
	KongfuBase(string name, Damage damage) :_name(name), _damage(damage){}
	~KongfuBase(){};

	virtual const string& getName()
	{
		return this->_name;
	}

	virtual Damage& getDamage(){ return this->_damage; }
protected:
	string _name;
	Damage _damage;
};

class Fight :public KongfuBase
{
public:
	Fight(){};
	Fight(string name, Damage &damage):KongfuBase(name, damage){}
	~Fight(){}
private:

};

class Defense :public KongfuBase
{
public:
	Defense(){};
	Defense(string name, Damage &damage) :KongfuBase(name, damage){}
	~Defense(){};

private:

};
