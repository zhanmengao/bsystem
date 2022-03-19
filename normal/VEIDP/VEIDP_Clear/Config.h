#pragma once
#include<string>
#ifdef _WIN32
#include"../../../Setting/Readini/INIManager.hpp"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#endif
#include<string>


class Config
{
public:
	void ReloadConfig();
	void SaveVersion(const std::string& version, const std::string& url, const std::string& md5,int updateType);
	void SaveOrgcode(const std::string& orgcode);
public:
	//config.ini
    std::string IP;
    std::string Topic;
    std::string Group;
    std::string Mac;
    std::string InstructIp;
    std::string InstructTopic;

	std::string wanProduceIp;
	std::string wanProduceTopic;
	std::string wanConsuIp;
	std::string wanConsuTopic;
	std::string wanGroup;

    std::string organization;
	//veidp.ini
	int pantTime;
    std::string aidaPath;
	int bStartAida;
	int endTick;

private:
#ifdef _WIN32
	iniManager mConfig;
#endif
}; 
