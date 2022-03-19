#include"../StringTools/StrUtils.h"
#include<fstream>
#include<map>
struct ScriptData
{
	std::string name;
	char sp;
	std::string parameter;
	std::string end;
};
std::string ScriptHead;
std::vector<ScriptData> Scripts;
char buffer[1024] = { 0 };

ScriptData* GetScript(const std::string& name)
{
	for (auto it = Scripts.begin(); it != Scripts.end(); it++)
	{
		if (it->name == name)
		{
			return &(*it);
		}
	}
	return nullptr;
}
void ProcessLine(const std::string& sub, const char* end)
{
	vector<std::string> sv;
	//切割成字段名和参数
	char sp = ' ';
	StrUtils::Split(sub, sv, sp);
	if (sv.size() < 2)
	{
		sp = ':';
		sv.clear();
		StrUtils::Split(sub, sv, ':');
	}
	ScriptData data;
	data.sp = sp;
	data.parameter = sv[1];
	data.end = end;
	data.name = sv[0];
	Scripts.push_back(data);
}
int main()
{
	int line = 0;
	string str;
	ifstream ifs("InstallAD.ps1");
	while (ifs.getline(buffer, sizeof(buffer)))
	{
		if (line < 2)
		{
			ScriptHead.append(buffer);
			if (line == 0)
			{
				ScriptHead.append("\n");
			}
		}
		else
		{
			str += buffer;
			//满一行了
			size_t pos = str.find(" `");
			if (pos != string::npos)
			{
				std::string sub = str.substr(0, pos);
				ProcessLine(sub, " `");
				str.clear();
			}
		}
		line++;
	}
	ifs.close();
	//手动处理一下最后一行
	ProcessLine(str, "");
	auto pDomain = GetScript("-DomainName");
	if (!pDomain)
	{
		cout << "未发现域名字段！请检查 文件路径 或 正确性" << endl;
		return -1;
	}
	cout << "原域名为" << pDomain->parameter << endl;
	while (1)
	{
		cout << "请输入新域名" << endl;
		memset(buffer, 0, sizeof(buffer));
		cin.getline(buffer, sizeof(buffer));
		str = (buffer);
		if (str.empty())
		{
			cout << "输入为空，请重新输入" << endl;
			continue;
		}
		if (str[0] != '\"')
		{
			str = "\"" + str;
		}
		if (str[str.size() - 1] != '\"')
		{
			str.push_back('\"');
		}
		pDomain->parameter = str;
		cout << "输入成功，新域名" << str << endl;
		break;
	}



	//统计完成，写文件
	ofstream ofs("InstallAD.ps1", ios::trunc);
	ofs.write(ScriptHead.c_str(), ScriptHead.size());
	ofs.put('\n');
	for (int i = 0; i < Scripts.size(); i++)
	{
		auto& info = Scripts[i];
		ofs.write(info.name.c_str(), info.name.size());
		ofs.put(info.sp);
		ofs.write(info.parameter.c_str(), info.parameter.size());
		ofs.write(info.end.c_str(), info.end.size());

		if (i != Scripts.size() - 1)
		{
			ofs.put('\n');
		}
	}
	cout << "生成脚本文件成功" << endl;
	return 0;
}