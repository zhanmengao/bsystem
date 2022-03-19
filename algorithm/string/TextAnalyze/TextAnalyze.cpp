#include<stdio.h>
#include<string.h>
#include<map>
#include<fstream>
#include<string>
#include<list>
#include<vector>
#include<sstream>
#include<iostream>
#include<set>
/*
Word	Count 	Lines
and	2	1   2
brillig	1	1
did	1	2
gimble	1	2
gyre	1	2
in	1	2
slithy	1	1
the	2	1   2
toves	1	1
twas	1	1
wabe	1	2
Total	13
*/
//统计单词，出现次数，出现位置
typedef std::list<int> lineList;
std::map<std::string, lineList> gStrCount;
static std::string ToLowper(const std::string& str)
{
	std::string ret(str.size(), 0);
	for (size_t i = 0; i < str.size(); i++)
	{
		ret[i] = tolower(str[i]);
	}
	return ret;
}
static std::string Trim(const std::string& str)
{
	std::string ret;
	static std::set<char> filter;
	static bool bInit = false;
	if (!bInit)
	{
		filter.insert('\t');
		filter.insert(',');
		filter.insert('.');
		filter.insert('\"');
		filter.insert('(');
		filter.insert(')');
		filter.insert('[');
		filter.insert(']');
		filter.insert(';');
		bInit = true;
	}

	for (auto it = str.begin(); it != str.end(); it++)
	{
		if (filter.find(*it) == filter.end())
		{
			ret.push_back(*it);
		}
	}
	return ret;
}
static void SplitA(const std::string& s, std::vector<std::string>& sv, const char flag = ' ')
{
	sv.clear();
	std::istringstream iss(s);
	std::string temp;

	while (std::getline(iss, temp, flag))
	{
		sv.push_back(temp);
	}

	return;
}
bool FileCount(const char* filePath)
{
	std::ifstream ifs;
	char buffer[8192] = { 0 };
	ifs.open(filePath);
	if (!ifs.is_open())
	{
		return false;
	}
	int line = 1;
	while (ifs.getline(buffer, sizeof(buffer)))
	{
		//开始解析文字
		std::vector<std::string> sv;
		SplitA(buffer, sv);
		for (auto it = sv.begin(); it != sv.end(); it++)
		{
			std::string str = Trim(*it);
			str = ToLowper(str);
			if (str.empty())
			{
				continue;
			}
			gStrCount[str].push_back(line);
		}
		memset(buffer, 0, sizeof(buffer));
		line++;
	}
	return true;
}

void PrintCoint()
{
	std::cout << "Word" << "	" << "Count" << "	" << "Lines" << std::endl;
	for (auto it = gStrCount.begin(); it != gStrCount.end(); it++)
	{
		std::cout << it->first << "	" << it->second.size() << "	";
		auto lit = it->second.begin();
		for (; lit != it->second.end(); lit++)
		{
			std::cout << *lit;
			std::cout << " ";
		}
		std::cout << std::endl;
	}
}
int main(int argc,char* argv[])
{
	if (argc < 2)
	{
		return -1;
	}

	for (int i = 1; i < argc; i++)
	{
		FileCount(argv[i]);
	}
	PrintCoint();
	return 0;
}