#include<iostream>
#include<string>
#include<vector>
using namespace std;

string myStrCat(const vector<string>& v1...)
{
	string str;
	for (unsigned i = 0; i<v1.size(); i++)
	{
		if (i != 0)
		{
			str=str+" ";
		}
		str += v1[i];
	}
	return str;
}
int main305()
{
	vector<string> v1;

	string s1;
	cout << "ÇëÊäÈën¸ö×Ö·û´®" << endl;
	while (cin >> s1)
	{
		v1.push_back(s1);
	}
	cout << myStrCat(v1);
	return 0;
}
