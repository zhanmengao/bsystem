#ifndef __DEFINE_H_
#define __DEFINE_H_

#include <set>
using namespace std;

class GlobalDefine
{
public:
	static std::string configPath;
	static std::string saveDataPath;
	static std::wstring fifoName;
	static bool allWatch;
	static bool watchDOS;
	static bool watchXLC;
	static bool watchPDF;
	static int wSendHour;
	static int wSendMin;
};

class GlobalData
{
public:
	static std::set<std::string> modifiedData;      //过滤重复修改的文件
};
#endif