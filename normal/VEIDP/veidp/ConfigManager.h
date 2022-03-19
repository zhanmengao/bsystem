#pragma once
#include<string>
#include"../../../Setting/Readini/INIManager.hpp"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
class ConfigManager
{
public:
	void ReloadConfig();
public:
	string IP;
	string Topic;
	string Group;
	string Mac;
	string InstructIp;
	string InstructTopic;
	int pantTime;
	int lastAidaSpac;
	string organization;

	int bStartAida;
	int aidaTick;
	int aidaSpac;

	int endTick;
private:
	iniManager mConfig;
};