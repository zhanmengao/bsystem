#pragma once
#include<string>
#include"../../../Setting/Readini/INIManager.hpp"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"

class Config
{
public:
	void ReloadConfig();
	void SaveVersion(const std::string& version, const std::string& url, const std::string& md5,int updateType);
	void SaveOrgcode(const std::string& orgcode);
public:
	//config.ini
	string IP;
	string Topic;
	string Group;
	string Mac;
	string InstructIp;
	string InstructTopic;

	std::string wanProduceIp;
	std::string wanProduceTopic;
	std::string wanConsuIp;
	std::string wanConsuTopic;
	std::string wanGroup;

	string organization;
	//veidp.ini
	int pantTime;
	string aidaPath;
	int bStartAida;
	int endTick;

private:
	iniManager mConfig;
}; 
