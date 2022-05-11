#pragma once
#include <string>  
using namespace std;

class SetCfg
{
public:
	SetCfg();
public:
	bool save();
	bool load();
	string disk;
	string shareFolder;
	string userDownload;
	string userPersonal;
};

SetCfg* get_config();
