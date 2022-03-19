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
	//�и���ֶ����Ͳ���
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
			//��һ����
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
	//�ֶ�����һ�����һ��
	ProcessLine(str, "");
	auto pDomain = GetScript("-DomainName");
	if (!pDomain)
	{
		cout << "δ���������ֶΣ����� �ļ�·�� �� ��ȷ��" << endl;
		return -1;
	}
	cout << "ԭ����Ϊ" << pDomain->parameter << endl;
	while (1)
	{
		cout << "������������" << endl;
		memset(buffer, 0, sizeof(buffer));
		cin.getline(buffer, sizeof(buffer));
		str = (buffer);
		if (str.empty())
		{
			cout << "����Ϊ�գ�����������" << endl;
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
		cout << "����ɹ���������" << str << endl;
		break;
	}



	//ͳ����ɣ�д�ļ�
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
	cout << "���ɽű��ļ��ɹ�" << endl;
	return 0;
}