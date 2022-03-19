#include "Definer.h"


std::string GlobalDefine::configPath("F:/Space/Safe/FileWatch/Config.ini");
std::string GlobalDefine::saveDataPath;
std::wstring GlobalDefine::fifoName;
bool 	  GlobalDefine::allWatch = false;
bool 	  GlobalDefine::watchDOS = false;
bool 	  GlobalDefine::watchXLC = false;
bool 	  GlobalDefine::watchPDF = false;
int 	  GlobalDefine::wSendHour;
int 	  GlobalDefine::wSendMin;



std::set<std::string> GlobalData::modifiedData;
