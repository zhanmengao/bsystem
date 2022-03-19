#include<iostream>
#include<string>
#include<vector>
using namespace std;

string& addApp(string &name, const string &b, const string &B, const string &e, const string &E)
{
	string s(B);
	s = s + b;
	name.insert(0, s);
	s = e;
	s = s + E;
	name.append(s);
	return name;
}

int main945()
{
	string name("Jimmy");
	string b("Mr.");
	string e("Jack");
	cout<< addApp(name, b, b, e, e);
	
	return 0;
}

