#pragma once
#include<map>

struct LRUValue
{
	int val;
	long long lru;
};
class LRUCache
{
public:
	LRUCache(int capacity) :cap(capacity)
	{

	}

	int get(int key)
	{
		lru++;
		auto it = mLRU.find(key);
		if (it != mLRU.end())
		{
			it->second.lru = lru;
		}
		return -1;
	}

	void put(int key, int value)
	{
		lru++;
		auto it = mLRU.find(key);
		if (it == mLRU.end())
		{
			LRUValue val;
			val.val = value;
			val.lru = lru;
			//��̭
			if (mLRU.size() >= cap)
			{
				delVal();
			}
			mLRU.insert(std::make_pair(key, val));
		}
		else
		{
			it->second.val = value;
			it->second.lru = lru;
		}
	}
private:
	void delVal()
	{
		long long minLRU = 0;
		auto minIt = mLRU.begin();
		for (auto it = mLRU.begin(); it != mLRU.end(); it++)
		{
			if (it->second.lru < minLRU)
			{
				minIt = it;
			}
		}
		mLRU.erase(minIt);
	}
private:
	std::map<int, struct LRUValue> mLRU;
	int cap;
	long long lru = 0;
};
/**
* Your LRUCache object will be instantiated and called as such:
* LRUCache* obj = new LRUCache(capacity);
* int param_1 = obj->get(key);
* obj->put(key,value);
*/

/*
����
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
���
[null, null, null, 1, null, -1, null, -1, 3, 4]

����
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // ������ {1=1}
lRUCache.put(2, 2); // ������ {1=1, 2=2}
lRUCache.get(1);    // ���� 1
lRUCache.put(3, 3); // �ò�����ʹ�ùؼ��� 2 ���ϣ������� {1=1, 3=3}
lRUCache.get(2);    // ���� -1 (δ�ҵ�)
lRUCache.put(4, 4); // �ò�����ʹ�ùؼ��� 1 ���ϣ������� {4=4, 3=3}
lRUCache.get(1);    // ���� -1 (δ�ҵ�)
lRUCache.get(3);    // ���� 3
lRUCache.get(4);    // ���� 4

��Դ�����ۣ�LeetCode��
���ӣ�https://leetcode-cn.com/problems/lru-cache
����Ȩ������������С���ҵת������ϵ�ٷ���Ȩ������ҵת����ע��������
*/