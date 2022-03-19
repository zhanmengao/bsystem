#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main03()
{
	map<string, size_t> m_WordCount;
	set<string> m_Exclude = { "The", "But", "And", "Or", "An", "A", "the", "but", "and", "or", "an", "a" };
	string word;
	while (cin >> word)
	{
		//���û����ӽ�ȥ
		if (m_Exclude.find(word) != m_Exclude.end())
		{
			cout << "�����ж���" << endl;
		}
		else
		{
			m_WordCount[word]++;
		}
	}
	for (auto it = m_WordCount.begin(); it != m_WordCount.end();)
	{
		cout << it->first << "������"<<it->second<<"��"<<endl;
		it++;
	}
   return 0;
}