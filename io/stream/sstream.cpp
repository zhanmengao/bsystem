#include<sstream>
#include<vector>
#include<iostream>
using namespace std;

struct PersonInfo
{
	string name;
	vector<string> phones;
};
void main33()
{
	string str("abc");
	stringstream ss1;
	stringstream ss2(str);						//保存对s的拷贝
	ss2.str();                      //拿到流中str的拷贝
	ss2.str(str);                             //将值拷贝到str中

	string line, word;
	vector<PersonInfo> pinfoVec;
	while (getline(cin, line))
	{
		PersonInfo person;
		istringstream iss(line);
		iss >> person.name;
		while (iss >> word)
		{
			person.phones.push_back(word);
		}
		pinfoVec.push_back(person);
	}

	//遍历vec
	for (const PersonInfo& entry : pinfoVec)
	{
		ostringstream formatted;
		for (const string& nums : entry.phones)
		{
			formatted << "	" << nums;
		}
		cout << entry.name << ":" << formatted.str() << endl;
	}
}