#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
using namespace std;

int main04()
{
	map<string, size_t> m_WordCount;
	set<string> m_Exclude = { "The", "But", "And", "Or", "An", "A", "the", "but", "and", "or", "an", "a" };
	string word;
	while (cin >> word)
	{
		//如果没有添加进去
		if (m_Exclude.find(word) != m_Exclude.end())
		{
			cout << "名字有毒！" << endl;
		}
		else
		{
			for (auto &it = word.begin(); it != word.end();)
			{
				if (isalpha(*it))
				{
					//统一输出小写
					*it=tolower(*it);
					it++;
				}
				//去除标点
				else if (ispunct(*it))
				{
					it=word.erase(it,it+1);
				}
			}
			m_WordCount[word]++;	
		}
	}
	for (auto it = m_WordCount.begin(); it != m_WordCount.end();)
	{
		cout << it->first << "出现了" << it->second << "次" << endl;
		it++;
	}
   return 0;
}