#pragma once
#include<thread>
#include<mutex>
#include<stdint.h>
#include<limits.h>
class CppThread
{
public:

};


//��������ͼ��һ���������������ڸò����ѱ��Ͳ����ʱ�������ǲ�����ġ�
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
	unsigned long const mHierarchyValue;									 //��������ĵȼ�
	unsigned long previousHierarchyValue;									//��һ���������ݻ���  
private:
	static  unsigned long NowHierarchyValue;								//��ǰ������static			
	static std::mutex StaticMutex;
private:
	bool CheckForHierarchyViolation();
	void UpdateHierarchyValue();
};
