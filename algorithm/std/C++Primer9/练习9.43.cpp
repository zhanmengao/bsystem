#include<iostream>
#include<string>
#include<vector>
using namespace std;

bool ReplaceString(string &s, const string& oldVal, const string& newVal);
int main943()
{
	string s("woaic++");
	string oldVal("c++");
	string newVal("guoyue");
	ReplaceString(s, oldVal, newVal);
	cout<<s<<endl;
	return 0;
}
bool ReplaceString(string &s,const string& oldVal, const string& newVal)
{
	unsigned pos = 0;
	while (pos<s.size())
	{
		//Ѱ����һ��oldVal
		pos=s.find(oldVal, pos);
		if (pos == -1 || pos >= s.size())
		{
			break;
		}
		//�滻
		s.erase(pos, oldVal.size());
		//����
		s.insert(pos,newVal);
		pos++;
	}
	return true;
	
}
