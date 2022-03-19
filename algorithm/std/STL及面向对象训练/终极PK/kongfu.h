#include<string>
#include <iostream>

using namespace std;

//ս������
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
	double getValue(){ return _value; }	//ս��ֵ
	int getMode(){ return _mode; }	//ս��ģʽ  Ѫ��ֵ/�ٷֱ�
	bool isSustained(){ return _isSustained; } //�Ƿ��г���Ч��
	int sustainedRound(){ return _round; } //����Ч���غ�

private:
	double _value;
	int _mode;
	bool _isSustained;
	int _round;
};

//��ʽ�ӿ�
class IKongFu
{
public:
	virtual const string& getName() = 0;
	virtual Damage& getDamage() = 0;
};


//��ʽ����
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
