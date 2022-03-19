#pragma once
#include <string>
using namespace std;


//智能体基类
class BaseGameEntity
{
	friend class TelegramManager;
protected:
	BaseGameEntity(int id, string name = "");
public:
	const string& getName() const;
protected:
	int id;
	string name;
};