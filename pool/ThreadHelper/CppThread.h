#pragma once
#include<thread>
#include<mutex>
#include<stdint.h>
#include<limits.h>
class CppThread
{
public:

};


//当代码试图对一个互斥量上锁，在该层锁已被低层持有时，上锁是不允许的。
class HierarchicalMutex
{
public:
	explicit HierarchicalMutex(unsigned long value) :mHierarchyValue(value), previousHierarchyValue(0)
	{}
	void Lock();
	void UnLock();								
	bool TryLock();
private:
	std::mutex mMutex;
	unsigned long const mHierarchyValue;									 //我这把锁的等级
	unsigned long previousHierarchyValue;									//上一把锁的数据缓存  
private:
	static  unsigned long NowHierarchyValue;								//当前锁级别static			
	static std::mutex StaticMutex;
private:
	bool CheckForHierarchyViolation();
	void UpdateHierarchyValue();
};
