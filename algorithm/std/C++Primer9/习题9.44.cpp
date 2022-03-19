#include<iostream>
#include<string>
#include<vector>
using namespace std;

bool replace_String(string &s, const string& oldVal, const string& newVal);

int main44()
{
	string s("woaic++");
	string oldVal("c++");
	string newVal("wife");
	replace_String(s, oldVal, newVal);
	cout << s << endl;
	return 0;
}
bool replace_String(string &s, const string& oldVal, const string& newVal)
{
	unsigned pos = 0;
	while (pos < s.size())
	{
		pos = s.find(oldVal,pos);
		if (pos == -1 || pos >= s.size())
		{
			break;
		}
		//Ìæ»»
		s.replace(pos, oldVal.size(), newVal);
	}
	
	return 1;

}
