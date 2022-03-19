#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

#include "set"

//可重复的set
void main1001()
{
	multiset<int> set1;
	int tmp = 0;

	printf("请输入multiset集合的值:");
	scanf("%d", &tmp);
	while (tmp != 0)
	{
		set1.insert(tmp);
		printf("请输入multiset集合的值:");
		scanf("%d", &tmp);
	}

	//遍历
	for (multiset<int>::iterator it=set1.begin(); it!=set1.end(); it++ )
	{
		cout << *it << " ";
	}
	cout << endl;

	while (!set1.empty())
	{
		multiset<int>::iterator it = set1.begin();
		cout << *it << " ";
		set1.erase(it);
	}
}