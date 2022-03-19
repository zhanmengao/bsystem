#include<iostream>
#include<string>
#include<vector>
using namespace std;

void findNum_of_first(const string& str)
{
	//找到每个数字
	string num = "0123456789";
	unsigned pos = 0;
	while (pos!=-1 || pos<str.size())
	{
		pos = str.find_first_of(num, pos);
		if (pos == -1)
		{
			break;
		}
		cout << str.at(pos);
		pos++;
	}
}
void findNum_last_first(const string& str)
{
	//找到每个数字
	string num = "0123456789";
	unsigned pos = str.size()-1;
	while (pos >=0)
	{
		pos = str.find_last_of(num, pos);
		if (pos == -1)
		{
			break;
		}
		cout << str.at(pos);
		pos--;
	}
}
void findchar_of_first(const string& str)
{
	//找到每个数字
	string num = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
	unsigned pos = 0;
	while (pos != -1 || pos < str.size())
	{
		pos = str.find(num, pos);
		if (pos == -1)
		{
			break;
		}
		cout << str.at(pos);
		pos--;
	}
}
void findchar_of_last(const string& str)
{
	//找到每个数字
	string num = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
	unsigned pos = 0;
	while (pos != -1|| pos<str.size())
	{
		pos = str.find_last_of(num, pos);
		if (pos == -1)
		{
			break;
		}
		cout << str.at(pos);
		pos++;
	}
}
int main947()
{
	string s("ab2c3d7R4E6");
	findchar_of_last(s);
	return 0;
}
