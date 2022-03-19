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
			//淘汰
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
输入
["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
[[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
输出
[null, null, null, 1, null, -1, null, -1, 3, 4]

解释
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // 缓存是 {1=1}
lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
lRUCache.get(1);    // 返回 1
lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
lRUCache.get(2);    // 返回 -1 (未找到)
lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
lRUCache.get(1);    // 返回 -1 (未找到)
lRUCache.get(3);    // 返回 3
lRUCache.get(4);    // 返回 4

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/lru-cache
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
*/