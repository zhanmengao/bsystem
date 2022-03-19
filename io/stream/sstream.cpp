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
	stringstream ss2(str);						//�����s�Ŀ���
	ss2.str();                      //�õ�����str�Ŀ���
	ss2.str(str);                             //��ֵ������str��

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

	//����vec
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