#include<iostream>
#include<string>
#include<vector>
using namespace std;

string& addIns(string &name, const string &b, const string &B, const string &e, const string &E)
{
	string s(B);
	s = s + b;
	name.insert(0, s);
	s = e;
	s = s + E;
	name.insert(name.size(), s);
	return name;
}
int main946()
{
	string name("Jimmy");
	string b("Mr.");
	string e("Jack");
	cout << addIns(name, b, b, e, e);

	return 0;
}
