#include<iostream>
#include<string>
#include<vector>
using namespace std;

bool Compare(const string& s1, const string& s2)
{
	if (s1 > s2)
	{
		cout << s1 << "��" << endl;
		return 1;
	}
	else if (s1 < s2)
	{
		cout << s2 << "��" << endl;
		return 0;
	}
	else{
		cout << "һ����" << endl;
		return 1;
	}
}
//
bool CompareSize(const string& s1, const string& s2)
{
	if (s1.size() > s2.size())
	{
		cout << s1 << "��" << endl;
		return 1;
	}
	else if (s1.size() < s2.size())
	{
		cout << s2 << "��" << endl;
		return 0;
	}
	else{
		cout << "һ����" << endl;
		return 1;
	}
}
int main304()
{
	string s1, s2;
	cout << "�����������ַ���"<<endl;
	cin >> s1>>s2;
	cout << Compare(s1, s2);

	return 0;
}
